namespace CustomExceptions
{
    public class FoundByIdException : Exception
    {
        public FoundByIdException(string message) : base($"Found by Id: {message}") { }
    }
    public class AddCelebrityException : Exception
    {
        public AddCelebrityException(string message) : base($"SaveChanges error: {message}") { }
    }
    public class SaveException : Exception
    {
        public SaveException(string message) : base($"AddCelebrityException error: {message}") { }
    }
    public class DeleteCelebrityById : Exception
    {
        public DeleteCelebrityById(string message) : base($"DeleteCelebrity error: {message}") { }
    }
}
