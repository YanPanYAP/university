using BSTU.Results.Collection.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using BSTU.Results.Collection.Models;

namespace BSTU.Results.Collection.Services
{
    public class ResultsService : IResultsService
    {
        private readonly string _filePath;
        private static readonly SemaphoreSlim _semaphore = new SemaphoreSlim(1, 1);

        public ResultsService(ResultsOptions options)
        {
            _filePath = options.FilePath;
            InitializeFileIfNotExists();
        }

        private void InitializeFileIfNotExists()
        {
            if (!File.Exists(_filePath))
            {
                File.WriteAllText(_filePath, "[]");
            }
        }
        private async Task EnsureFileExistsAsync()
        {
            if (!File.Exists(_filePath))
            {
                await File.WriteAllTextAsync(_filePath, "[]");
            }
        }

        public async Task<IEnumerable<ResultItem>> GetAllAsync()
        {
            await _semaphore.WaitAsync();
            try
            {
                await EnsureFileExistsAsync();
                var json = await File.ReadAllTextAsync(_filePath);
                var items = JsonSerializer.Deserialize<List<ResultItem>>(json) ?? new List<ResultItem>();
                return items;
            }
            finally
            {
                _semaphore.Release();
            }
        }
        public async Task<ResultItem?> GetByIdAsync(int id)
        {

            var items = await GetAllAsync();
            return items.FirstOrDefault(x => x.Id == id);
        }

        public async Task<ResultItem> AddAsync(string value)
        {
            await _semaphore.WaitAsync();
            try
            {

                await EnsureFileExistsAsync();
                var json = await File.ReadAllTextAsync(_filePath);
                var items = JsonSerializer.Deserialize<List<ResultItem>>(json) ?? new List<ResultItem>();

                int newId = items.Any() ? items.Max(x => x.Id) + 1 : 1;

                var newItem = new ResultItem
                {
                    Id = newId,
                    Value = value
                };
                items.Add(newItem);

                var newJson = JsonSerializer.Serialize(items, new JsonSerializerOptions { WriteIndented = true });
                await File.WriteAllTextAsync(_filePath, newJson);

                return newItem;
            }
            finally { _semaphore.Release(); }
        }

        public async Task<ResultItem?> UpdateAsync(int id, string value)
        {
            await _semaphore.WaitAsync();
            try
            {

                await EnsureFileExistsAsync();
                var json = await File.ReadAllTextAsync(_filePath);
                var items = JsonSerializer.Deserialize<List<ResultItem>>(json) ?? new List<ResultItem>();

                var item = items.FirstOrDefault(x => x.Id == id);
                if (item == null)
                {
                    return null;
                }

                item.Value = value;

                var newJson = JsonSerializer.Serialize(items, new JsonSerializerOptions { WriteIndented = true });
                await File.WriteAllTextAsync(_filePath, newJson);

                return item;
            }
            finally { _semaphore.Release(); }
        }

        public async Task<ResultItem?> DeleteAsync(int id)
        {
            await _semaphore.WaitAsync();
            try
            {

                await EnsureFileExistsAsync();
                var json = await File.ReadAllTextAsync(_filePath);
                var items = JsonSerializer.Deserialize<List<ResultItem>>(json) ?? new List<ResultItem>();

                var item = items.FirstOrDefault(x => x.Id == id);

                if (item == null)
                {
                    return null;
                }

                items.Remove(item);

                var newJson = JsonSerializer.Serialize(items, new JsonSerializerOptions { WriteIndented = true });
                await File.WriteAllTextAsync(_filePath, newJson);

                return item;
            }
            finally
            {
                _semaphore.Release();
            }
        }
    }
}
