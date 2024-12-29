using System;
using System.IO;
using System.IO.Compression;

public class XXXLog
{
    private readonly string LogFilePath;

    public XXXLog(string baseDir)
    {
        LogFilePath = Path.Combine(baseDir, "xxxlogfile.txt");
    }

    public void WriteLog(string action, string details)
    {
        try
        {
            using (StreamWriter sw = new StreamWriter(LogFilePath, true))
            {
                sw.WriteLine($"{DateTime.Now}: {action} - {details}");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка записи в лог: {ex.Message}");
        }
    }

    public void ReadLog()
    {
        try
        {
            using (StreamReader sr = new StreamReader(LogFilePath))
            {
                Console.WriteLine("Содержимое лога:");
                Console.WriteLine(sr.ReadToEnd());
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка чтения лога: {ex.Message}");
        }
    }

    public void SearchInLog(string keyword)
    {
        try
        {
            Console.WriteLine($"Результаты поиска по ключевому слову \"{keyword}\":");
            using (StreamReader sr = new StreamReader(LogFilePath))
            {
                string line;
                while ((line = sr.ReadLine()) != null)
                {
                    if (line.Contains(keyword))
                        Console.WriteLine(line);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка поиска в логе: {ex.Message}");
        }
    }
}

public class XXXDiskInfo
{
    public void ShowFreeSpace()
    {
        foreach (var drive in DriveInfo.GetDrives())
        {
            if (drive.IsReady)
                Console.WriteLine($"Диск {drive.Name}: Свободное место - {drive.AvailableFreeSpace / 1_048_576} МБ");
        }
    }

    public void ShowFileSystemInfo()
    {
        foreach (var drive in DriveInfo.GetDrives())
        {
            if (drive.IsReady)
                Console.WriteLine($"Диск {drive.Name}: Файловая система - {drive.DriveFormat}");
        }
    }

    public void ShowDiskDetails()
    {
        foreach (var drive in DriveInfo.GetDrives())
        {
            if (drive.IsReady)
            {
                Console.WriteLine($"Диск {drive.Name}");
                Console.WriteLine($"Общий объем: {drive.TotalSize / 1_048_576} МБ");
                Console.WriteLine($"Доступное место: {drive.AvailableFreeSpace / 1_048_576} МБ");
                Console.WriteLine($"Метка тома: {drive.VolumeLabel}");
            }
        }
    }
}

public class XXXFileInfo
{
    public void ShowFileInfo(string filePath)
    {
        try
        {
            var fileInfo = new FileInfo(filePath);
            if (fileInfo.Exists)
            {
                Console.WriteLine($"Полный путь: {fileInfo.FullName}");
                Console.WriteLine($"Размер: {fileInfo.Length} байт");
                Console.WriteLine($"Расширение: {fileInfo.Extension}");
                Console.WriteLine($"Имя: {fileInfo.Name}");
                Console.WriteLine($"Дата создания: {fileInfo.CreationTime}");
                Console.WriteLine($"Дата изменения: {fileInfo.LastWriteTime}");
            }
            else
            {
                Console.WriteLine("Файл не существует.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка получения информации о файле: {ex.Message}");
        }
    }
}

public class XXXDirInfo
{
    public void ShowDirectoryInfo(string dirPath)
    {
        try
        {
            var dirInfo = new DirectoryInfo(dirPath);
            if (dirInfo.Exists)
            {
                Console.WriteLine($"Директория: {dirInfo.FullName}");
                Console.WriteLine($"Дата создания: {dirInfo.CreationTime}");
                Console.WriteLine($"Количество файлов: {dirInfo.GetFiles().Length}");
                Console.WriteLine($"Количество поддиректорий: {dirInfo.GetDirectories().Length}");

                Console.WriteLine("Список родительских директорий:");
                var parent = dirInfo.Parent;
                while (parent != null)
                {
                    Console.WriteLine($"Родитель: {parent.FullName}");
                    parent = parent.Parent;
                }
            }
            else
            {
                Console.WriteLine("Директория не существует.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка получения информации о директории: {ex.Message}");
        }
    }
}

public class XXXFileManager
{
    private readonly string baseDir;

    public XXXFileManager(string baseDir)
    {
        this.baseDir = baseDir;
    }

    public void InspectDrive()
    {
        try
        {
            string inspectDir = Path.Combine(baseDir, "XXXInspect");
            Directory.CreateDirectory(inspectDir);

            string dirInfoFile = Path.Combine(inspectDir, "xxxdirinfo.txt");

            using (StreamWriter sw = new StreamWriter(dirInfoFile))
            {
                foreach (var dir in Directory.GetDirectories(baseDir))
                    sw.WriteLine($"Директория: {dir}");

                foreach (var file in Directory.GetFiles(baseDir))
                    sw.WriteLine($"Файл: {file}");
            }

            Console.WriteLine($"Создан файл {dirInfoFile} с информацией о директориях и файлах.");
            File.Copy(dirInfoFile, Path.Combine(inspectDir, "xxxdirinfo_copy.txt"));
            File.Delete(dirInfoFile);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при инспекции диска: {ex.Message}");
        }
    }

    public void ManageFiles(string extension)
    {
        try
        {
            string filesDir = Path.Combine(baseDir, "XXXFiles");
            Directory.CreateDirectory(filesDir);

            foreach (var file in Directory.GetFiles(baseDir, $"*{extension}"))
            {
                File.Copy(file, Path.Combine(filesDir, Path.GetFileName(file)));
            }

            Directory.Move(filesDir, Path.Combine(baseDir, "XXXInspect", "XXXFiles"));
            Console.WriteLine($"Файлы перемещены в директорию {Path.Combine(baseDir, "XXXInspect", "XXXFiles")}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при управлении файлами: {ex.Message}");
        }
    }

    public void ArchiveFiles()
    {
        try
        {
            string sourceDir = Path.Combine(baseDir, "XXXInspect", "XXXFiles");
            string archivePath = Path.Combine(baseDir, "archive.zip");
            ZipFile.CreateFromDirectory(sourceDir, archivePath);
            Console.WriteLine($"Создан архив {archivePath}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка создания архива: {ex.Message}");
        }
    }

    public void ExtractArchive()
    {
        try
        {
            string archivePath = Path.Combine(baseDir, "archive.zip");
            string extractPath = Path.Combine(baseDir, "ExtractedFiles");
            ZipFile.ExtractToDirectory(archivePath, extractPath);
            Console.WriteLine($"Архив {archivePath} разархивирован в {extractPath}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка разархивирования: {ex.Message}");
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        string baseDir = "C:\\oop12";

        var log = new XXXLog(baseDir);
        log.WriteLog("Старт программы", "Запуск приложения");

        var diskInfo = new XXXDiskInfo();
        diskInfo.ShowFreeSpace();
        diskInfo.ShowFileSystemInfo();
        diskInfo.ShowDiskDetails();

        var fileInfo = new XXXFileInfo();
        fileInfo.ShowFileInfo(Path.Combine(baseDir, "example.txt"));

        var dirInfo = new XXXDirInfo();
        dirInfo.ShowDirectoryInfo(baseDir);

        var fileManager = new XXXFileManager(baseDir);
        fileManager.InspectDrive();
        fileManager.ManageFiles(".txt");
        fileManager.ArchiveFiles();
        fileManager.ExtractArchive();

        log.WriteLog("Завершение программы", "Программа завершена.");
    }
}
