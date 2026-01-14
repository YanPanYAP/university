using BSTU.Results.Authenticate.Models;
using Microsoft.AspNetCore.Http;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace BSTU.Results.Authenticate.Services
{
    public interface IAuthenticateService
    {
        Task<bool> SignInAsync(HttpContext htppContext, LoginRequest request);
        Task SignOutAsync(HttpContext htppContext);
    }
}
