using BSTU.Results.Collection.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BSTU.Results.Collection.Services
{
    public interface IResultsService
    {
        Task<IEnumerable<ResultItem>> GetAllAsync();
        Task<ResultItem?> GetByIdAsync(int id);
        Task<ResultItem> AddAsync(string value);
        Task<ResultItem?> UpdateAsync(int id, string value);
        Task<ResultItem?> DeleteAsync(int id);
    }
}
