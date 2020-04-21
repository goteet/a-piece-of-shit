using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;

namespace mock_resource_loading_test
{
    public class LimitedConcurrencyLevelTaskScheduler : TaskScheduler
    {
        // Indicates whether the current thread is processing work items.
        [ThreadStatic]
        private static bool _currentThreadIsProcessingItems;

        // The list of tasks to be executed 
        private readonly LinkedList<Task> _tasks = new LinkedList<Task>(); // protected by lock(_tasks)

        // The maximum concurrency level allowed by this scheduler. 
        private readonly int _maxDegreeOfParallelism;

        // Indicates whether the scheduler is currently processing work items. 
        private int _delegatesQueuedOrRunning = 0;

        // Creates a new instance with the specified degree of parallelism. 
        public LimitedConcurrencyLevelTaskScheduler(int maxDegreeOfParallelism)
        {
            if (maxDegreeOfParallelism < 1) throw new ArgumentOutOfRangeException("maxDegreeOfParallelism");
            _maxDegreeOfParallelism = maxDegreeOfParallelism;
        }

        // Queues a task to the scheduler. 
        protected sealed override void QueueTask(Task task)
        {
            // Add the task to the list of tasks to be processed.  If there aren't enough 
            // delegates currently queued or running to process tasks, schedule another. 
            lock (_tasks)
            {
                _tasks.AddLast(task);
                if (_delegatesQueuedOrRunning < _maxDegreeOfParallelism)
                {
                    ++_delegatesQueuedOrRunning;
                    NotifyThreadPoolOfPendingWork();
                }
            }
        }

        // Inform the ThreadPool that there's work to be executed for this scheduler. 
        private void NotifyThreadPoolOfPendingWork()
        {
            ThreadPool.UnsafeQueueUserWorkItem(_ =>
            {
                // Note that the current thread is now processing work items.
                // This is necessary to enable inlining of tasks into this thread.
                _currentThreadIsProcessingItems = true;
                try
                {
                    // Process all available items in the queue.
                    while (true)
                    {
                        Task item;
                        lock (_tasks)
                        {
                            // When there are no more items to be processed,
                            // note that we're done processing, and get out.
                            if (_tasks.Count == 0)
                            {
                                --_delegatesQueuedOrRunning;
                                break;
                            }

                            // Get the next item from the queue
                            item = _tasks.First.Value;
                            _tasks.RemoveFirst();
                        }

                        // Execute the task we pulled out of the queue
                        base.TryExecuteTask(item);
                    }
                }
                // We're done processing items on the current thread
                finally { _currentThreadIsProcessingItems = false; }
            }, null);
        }

        // Attempts to execute the specified task on the current thread. 
        protected sealed override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued)
        {
            // If this thread isn't already processing a task, we don't support inlining
            if (!_currentThreadIsProcessingItems) return false;

            // If the task was previously queued, remove it from the queue
            if (taskWasPreviouslyQueued)
                // Try to run the task. 
                if (TryDequeue(task))
                    return base.TryExecuteTask(task);
                else
                    return false;
            else
                return base.TryExecuteTask(task);
        }

        // Attempt to remove a previously scheduled task from the scheduler. 
        protected sealed override bool TryDequeue(Task task)
        {
            lock (_tasks) return _tasks.Remove(task);
        }

        // Gets the maximum concurrency level supported by this scheduler. 
        public sealed override int MaximumConcurrencyLevel { get { return _maxDegreeOfParallelism; } }

        // Gets an enumerable of the tasks currently scheduled on this scheduler. 
        protected sealed override IEnumerable<Task> GetScheduledTasks()
        {
            bool lockTaken = false;
            try
            {
                Monitor.TryEnter(_tasks, ref lockTaken);
                if (lockTaken) return _tasks;
                else throw new NotSupportedException();
            }
            finally
            {
                if (lockTaken) Monitor.Exit(_tasks);
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            SingleThreadLoadingMaterial();
            Console.WriteLine("Single Thread Loading Time : {0} milliseconds.",
                 stopwatch.ElapsedMilliseconds);

            stopwatch.Restart();
            TaskGraphLoadingMaterial();
            Console.WriteLine("Task Graph Loading Time : {0} milliseconds.",
                 stopwatch.ElapsedMilliseconds);
        }

        static void SingleThreadLoadingMaterial()
        {
            MockMaterialRes res = new MockMaterialRes();
            //Read Material File into memory.

            Console.WriteLine("Read Material File.");
            MaterialFileData matFileData = res.ReadFileData<MaterialFileData>();

            Console.WriteLine("Parse Material Data.");
            MaterialData matData = res.ParseMaterialInfo(matFileData);

            Console.WriteLine("Create Material Instance.");
            Material material = res.CreateMaterial(matData);

            Console.WriteLine("Start Parsing Texture List.");
            string textureFilePath;
            while (res.ParseNextTextureFilePath(out textureFilePath))
            {
                Console.WriteLine("\n  Find used Texture: {0}", textureFilePath);

                Console.WriteLine("  Read Image File.");
                ImageFileData imgFileData = res.ReadFileData<ImageFileData>();

                Console.WriteLine("  Parse Image Data.");
                ImageData imgData = res.ParseImageInfo(imgFileData);

                Console.WriteLine("  Create Texture from Image Data.");
                Texture texture = res.CreateTexture(textureFilePath, imgData);

                Console.WriteLine("  Assemble Texture into Material Instance.");
                material.AddTexture(texture);
            }

            material.PrintTextureNames();
            Console.WriteLine("Loading Finished.");
        }

        class SubTaskParam
        {
            public string TextureFilePath;
            public MaterialTaskGroup TaskGroup;
        }

        class MaterialTaskGroup
        {
            public LimitedConcurrencyLevelTaskScheduler SchedulerIO = new LimitedConcurrencyLevelTaskScheduler(1);
            public LimitedConcurrencyLevelTaskScheduler SchedulerMain = new LimitedConcurrencyLevelTaskScheduler(1);
            public CancellationTokenSource CTS = new CancellationTokenSource();
            public MockMaterialRes res = new MockMaterialRes();

            public Task finalAssmebleTask = null;

            //blackboard
            MaterialFileData matFileData = null;
            MaterialData matData = null;
            List<Texture> textures = new List<Texture>();
            Material material = new Material();

            public void ReadMaterialData()
            {
                Console.WriteLine("Read Material File.");
                matFileData = res.ReadFileData<MaterialFileData>();
            }

            public void ParseUsedTextureDatas()
            {
                Console.WriteLine("Start Parsing Texture List.");
                string textureFilePath;
                while (res.ParseNextTextureFilePath(out textureFilePath))
                {
                    Console.WriteLine("  Find used Texture : {0}.", textureFilePath);

                    SubTaskParam param = new SubTaskParam();
                    param.TextureFilePath = textureFilePath;
                    param.TaskGroup = this;

                    //Task<ImageFileData> readImageFileTask = 
                    Task<ImageFileData>.Factory.StartNew(arg =>
                    {
                        SubTaskParam p = arg as SubTaskParam;
                        MaterialTaskGroup argTaskGroup = param.TaskGroup;

                        Console.WriteLine("  Read Image File : {0}", p.TextureFilePath);
                        ImageFileData imgFileData = res.ReadFileData<ImageFileData>();
                        return imgFileData;

                    }, param, CTS.Token, TaskCreationOptions.AttachedToParent, SchedulerIO)
                    .ContinueWith((antecedent, arg) =>
                    {
                        SubTaskParam p = arg as SubTaskParam;
                        ImageFileData imgFileData = antecedent.Result as ImageFileData;

                        Console.WriteLine("  Parse Image Data : {0}", p.TextureFilePath);
                        ImageData imgData = res.ParseImageInfo(imgFileData);
                        return imgData;

                    }, param, CTS.Token, TaskContinuationOptions.AttachedToParent, TaskScheduler.Default)
                    .ContinueWith((antecedent, arg) =>
                    {
                        SubTaskParam p = arg as SubTaskParam;
                        ImageData imgFileData = antecedent.Result as ImageData;

                        Console.WriteLine("  Create Texture from Image Data : {0}", p.TextureFilePath);
                        Texture texture = res.CreateTexture(p.TextureFilePath, imgFileData);
                        textures.Add(texture);

                    }, param, CTS.Token, TaskContinuationOptions.AttachedToParent, SchedulerMain);
                }
            }

            public void ParseMaterialData()
            {
                Console.WriteLine("Parse Material Data.");
                matData = res.ParseMaterialInfo(matFileData);
            }

            public void AssembleTexutureIntoMaterial()
            {
                Console.WriteLine("  Assemble Texture into Material Instance.");
                foreach (Texture texture in textures)
                {
                    material.AddTexture(texture);
                }
                textures.Clear();
            }

            public void ShowMaterial()
            {
                material.PrintTextureNames();
            }
        }


        static void TaskGraphLoadingMaterial()
        {
            MaterialTaskGroup taskGroup = new MaterialTaskGroup();
            Task readMaterialFileDataTask = Task.Factory.StartNew(arg =>
            {
                MaterialTaskGroup argTaskGroup = arg as MaterialTaskGroup;
                argTaskGroup.ReadMaterialData();
            }, taskGroup, taskGroup.CTS.Token, TaskCreationOptions.None, taskGroup.SchedulerIO);

            Task parseMaterialFileDataTask = readMaterialFileDataTask.ContinueWith((antecedent, arg) =>
            {
                MaterialTaskGroup argTaskGroup = arg as MaterialTaskGroup;
                argTaskGroup.ParseMaterialData();
            }, taskGroup, taskGroup.CTS.Token, TaskContinuationOptions.None, TaskScheduler.Default);

            Task parseUsedTextureTask = readMaterialFileDataTask.ContinueWith((antecedent, arg) =>
            {
                MaterialTaskGroup argTaskGroup = arg as MaterialTaskGroup;
                argTaskGroup.ParseUsedTextureDatas();
            }, taskGroup, taskGroup.CTS.Token, TaskContinuationOptions.None, TaskScheduler.Default);

            Task JoinTask = Task.WhenAll(new List<Task> { parseMaterialFileDataTask, parseUsedTextureTask });

            taskGroup.finalAssmebleTask = JoinTask.ContinueWith((antecedent, arg) =>
            {
                MaterialTaskGroup argTaskGroup = arg as MaterialTaskGroup;
                argTaskGroup.AssembleTexutureIntoMaterial();
            }, taskGroup, taskGroup.CTS.Token, TaskContinuationOptions.None, taskGroup.SchedulerMain);

            Task.WaitAll(taskGroup.finalAssmebleTask);

            taskGroup.ShowMaterial();

            Console.WriteLine("Loading Finished.");
        }
    }
}
