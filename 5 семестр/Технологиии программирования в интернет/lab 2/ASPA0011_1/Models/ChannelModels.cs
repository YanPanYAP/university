namespace ASPA0011_1.Models
{
    public class ChannelInfo
    {
        public Guid Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public string Status { get; set; } = string.Empty;
        public int Count { get; set; }
        public DateTime Created { get; set; }
    }

    public class CreateChannelRequest
    {
        public string Name { get; set; } = string.Empty;
        public int Capacity { get; set; } = 100;
    }

    public class ChangeStatusRequest
    {
        public string Command { get; set; } = string.Empty; // "open", "close", "suspend", "resume"
        public Guid? ChannelId { get; set; }
    }

    public class DeleteChannelRequest
    {
        public Guid? ChannelId { get; set; }
        public bool DeleteAll { get; set; }
        public string? State { get; set; }
    }

    public class QueueOperationRequest
    {
        public Guid ChannelId { get; set; }
        public string Operation { get; set; } = string.Empty; // "enqueue", "dequeue", "peek"
        public object? Data { get; set; }
    }

    public class QueueOperationResponse
    {
        public bool Success { get; set; }
        public object? Data { get; set; }
        public string Message { get; set; } = string.Empty;
    }

    public class AppSettings
    {
        public int WaitEnqueue { get; set; } = 30;
    }
}