namespace CustomExceptions
{

    public class SurnameIsNull : Exception
    {
        public SurnameIsNull(string message) : base($"Found by Id: {message}") { }
    }
    public class SurnameEquals : Exception
    {
        public SurnameEquals(string message) : base($"Found by Id: {message}") { }
    }
    public class CelebryIsNull : Exception
    {
        public CelebryIsNull(string message) : base($"Found by Id: {message}") { }
    }
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

    public class PutCelebrityById : Exception
    {
        public PutCelebrityById(string message) : base($"PutCelebrity error: {message}") { }
    }

    public class PhotoPathException : Exception
    {
        public PhotoPathException(string message) : base($"PhotoPath error: {message}") { }
    }
}
