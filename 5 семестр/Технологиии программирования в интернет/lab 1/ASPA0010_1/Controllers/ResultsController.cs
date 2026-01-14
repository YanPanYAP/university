using BSTU.Results.Authenticate.Models;
using BSTU.Results.Authenticate.Services;
using BSTU.Results.Collection.Services;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity.Data;
using Microsoft.AspNetCore.Mvc;

namespace ASPA0010_1.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ResultsController : ControllerBase
    {
        private readonly IResultsService _resultService;
        private readonly IAuthenticateService _authenticateService;

        public ResultsController(IResultsService resultService, IAuthenticateService authenticateService)
        {
            _resultService = resultService;
            _authenticateService = authenticateService;
        }

        // ------------------------ Методы работы с результатами ------------------------

        [HttpGet]
        [Authorize(Roles = "READER,WRITER")]
        public async Task<IActionResult> GetAll()
        {
            var items = await _resultService.GetAllAsync();

            if (!items.Any())
            {
                return NoContent(); // 204 No Content
            }

            return Ok(items);
        }

        [HttpGet("{id:int}")]
        [Authorize(Roles = "READER,WRITER")]
        public async Task<IActionResult> GetById(int id)
        {
            var item = await _resultService.GetByIdAsync(id);

            if (item == null)
            {
                return NotFound(); // 404
            }

            return Ok(item);
        }

        [HttpPost]
        [Authorize(Roles = "WRITER")]
        public async Task<IActionResult> Add([FromBody] BodyRequest request)
        {
            if (string.IsNullOrWhiteSpace(request.Value))
            {
                return BadRequest("Value is required"); // 400
            }

            var newItem = await _resultService.AddAsync(request.Value);

            // 201 Created 
            return Created($"/api/Results/{newItem.Id}", newItem);
        }

        [HttpPut("{id:int}")]
        [Authorize(Roles = "WRITER")]
        public async Task<IActionResult> Update(int id, [FromBody] BodyRequest request)
        {
            if (string.IsNullOrWhiteSpace(request.Value))
                return BadRequest("Value is required");

            var updateItem = await _resultService.UpdateAsync(id, request.Value);

            if (updateItem == null)
            {
                return NotFound(); // 404
            }

            return Ok(updateItem);
        }

        [HttpDelete("{id:int}")]
        [Authorize(Roles = "WRITER")]
        public async Task<IActionResult> Delete(int id)
        {
            var deletedItem = await _resultService.DeleteAsync(id);

            if (deletedItem == null)
            {
                return NotFound();  // 404
            }
            return Ok(deletedItem);
        }

        // ------------------------ Методы аутентификации ------------------------

        [HttpPost("SignIn")]
        [AllowAnonymous]
        public async Task<IActionResult> SignIn([FromBody] BSTU.Results.Authenticate.Models.LoginRequest request)
        {
            if (string.IsNullOrWhiteSpace(request.Login) || string.IsNullOrWhiteSpace(request.Password))
                return BadRequest("Login and password are requeired"); // 400 

            var success = await _authenticateService.SignInAsync(HttpContext, request);

            if (!success)
                return NotFound("User not found or invalid creds"); // 404 

            return Ok(new { message = "Signed in successfuly" }); // 200
        }

        [HttpGet("SignOut")]
        [Authorize]
        public async Task<IActionResult> SignOut()
        {
            await _authenticateService.SignOutAsync(HttpContext);

            return Ok(new { message = "Signed out Successfully" });
        }
    }

    // Класс для передачи значения при добавлении/обновлении результатов
    public class BodyRequest
    {
        public string Value { get; set; } = string.Empty;
    }
}
