using System.Threading;
using System.Collections.Generic;
using System;

namespace mock_resource_loading_test
{
    class ImageData
    { }

    class MaterialData
    { }

    class MaterialFileData
    { }

    class ImageFileData
    { }

    class Texture
    {
        public Texture(string filePath)
        {
            FilePath = filePath;
        }
        public string FilePath;
    }

    class Material
    {
        public List<Texture> Textures = new List<Texture>();

        public void AddTexture(Texture tex)
        {
            Textures.Add(tex);
        }

        public void PrintTextureNames()
        {
            int i = 0;
            foreach (Texture texture in Textures)
            {
                Console.WriteLine("texture {0} : {1}", i++, texture.FilePath);
            }
        }
    }

    class MockMaterialRes
    {
        static string[] kUsedTextureList = new string[]
        {
            "texture0",
            "texture1",
            "texture2",
            "texture3",
            "texture4"
        };

        static readonly int kNumTextureUsed = kUsedTextureList.Length;

        int readedTextureFilePath = 0;

        public T ReadFileData<T>() where T : new()
        {
            Thread.Sleep(1000);
            return new T();
        }

        public bool ParseNextTextureFilePath(out string textureFilePath)
        {
            if (readedTextureFilePath >= kNumTextureUsed)
            {
                textureFilePath = "";
                return false;
            }
            else
            {
                Thread.Sleep(50);
                textureFilePath = kUsedTextureList[readedTextureFilePath];
                readedTextureFilePath += 1;
                return true;
            }
        }

        public MaterialData ParseMaterialInfo(MaterialFileData fileData)
        {
            Thread.Sleep(50);
            return new MaterialData();
        }

        public ImageData ParseImageInfo(ImageFileData fileData)
        {
            Thread.Sleep(500);
            return new ImageData();
        }

        public Material CreateMaterial(MaterialData data)
        {
            Thread.Sleep(50);
            return new Material();
        }

        public Texture CreateTexture(string filePath, ImageData data)
        {
            Thread.Sleep(50);
            return new Texture(filePath);
        }
    }
}
