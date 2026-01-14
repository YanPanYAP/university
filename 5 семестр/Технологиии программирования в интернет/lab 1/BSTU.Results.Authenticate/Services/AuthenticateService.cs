using BSTU.Results.Authenticate.Data;
using BSTU.Results.Authenticate.Models;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Http;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Security.Claims;
using System.Threading.Tasks;

namespace BSTU.Results.Authenticate.Services
{
    public class AuthenticateService : IAuthenticateService
    {
        public async Task<bool> SignInAsync(HttpContext httpContext, LoginRequest request)
        {

            var user = UserStore.FindUser(request.Login, request.Password);

            if (user == null)
            {
                return false;
            }

            var claims = new List<Claim>
            {
                new Claim(ClaimTypes.Name, user.Login),

                new Claim(ClaimTypes.Role, user.Role),
            };

            var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);

            // свойства аутентификации
            var authProperties = new AuthenticationProperties
            {
                IsPersistent = true,
                ExpiresUtc = DateTime.UtcNow.AddHours(1)
            };

            await httpContext.SignInAsync(
                CookieAuthenticationDefaults.AuthenticationScheme,
                new ClaimsPrincipal(claimsIdentity),
                authProperties
            );

            return true;
        }

        public async Task SignOutAsync(HttpContext httpContext)
        {
            await httpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
        }
    }
}
