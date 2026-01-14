using ASPA0011_1.Models;
using Microsoft.Extensions.Options;
using System.Collections.Concurrent;
using System.Threading.Channels;

namespace ASPA0011_1.Services
{
    public class ChannelService : IChannelService
    {
        private readonly ConcurrentDictionary<Guid, ChannelWrapper> _channels = new();
        private readonly ILogger<ChannelService> _logger;
        private readonly AppSettings _settings;

        public ChannelService(ILogger<ChannelService> logger, IOptions<AppSettings> settings)
        {
            _logger = logger;
            _settings = settings.Value;
        }

        public Task<List<ChannelInfo>> GetAllChannelsAsync()
        {
            _logger.LogTrace("Getting all channels");

            var channels = _channels.Values.Select(c => new ChannelInfo
            {
                Id = c.Id,
                Name = c.Name,
                Status = c.Status,
                Count = c.GetCount(),
                Created = c.Created
            }).ToList();

            _logger.LogDebug("Retrieved {Count} channels", channels.Count);
            return Task.FromResult(channels);
        }

        public Task<ChannelInfo?> GetChannelAsync(Guid id)
        {
            _logger.LogTrace("Getting channel {ChannelId}", id);

            if (_channels.TryGetValue(id, out var channel))
            {
                var info = new ChannelInfo
                {
                    Id = channel.Id,
                    Name = channel.Name,
                    Status = channel.Status,
                    Count = channel.GetCount(),
                    Created = channel.Created
                };

                _logger.LogDebug("Found channel {ChannelId} with status {Status}", id, channel.Status);
                return Task.FromResult<ChannelInfo?>(info);
            }

            _logger.LogError("Channel {ChannelId} not found", id);
            return Task.FromResult<ChannelInfo?>(null);
        }

        public Task<ChannelInfo> CreateChannelAsync(CreateChannelRequest request)
        {
            _logger.LogTrace("Creating channel with name {Name} and capacity {Capacity}", request.Name, request.Capacity);

            var id = Guid.NewGuid();
            var channel = new ChannelWrapper(id, request.Name, request.Capacity);

            _channels.TryAdd(id, channel);

            _logger.LogInformation("Created channel {ChannelId} with name {Name}", id, request.Name);

            var result = new ChannelInfo
            {
                Id = id,
                Name = request.Name,
                Status = "ACTIVE",
                Count = 0,
                Created = DateTime.UtcNow
            };

            return Task.FromResult(result);
        }

        public async Task<List<ChannelInfo>> ChangeChannelStatusAsync(ChangeStatusRequest request)
        {
            _logger.LogTrace("Changing channel status with command {Command}", request.Command);

            var channelsToUpdate = request.ChannelId.HasValue
                ? _channels.Values.Where(c => c.Id == request.ChannelId.Value).ToList()
                : _channels.Values.ToList();

            foreach (var channel in channelsToUpdate)
            {
                var oldStatus = channel.Status;

                switch (request.Command.ToLower())
                {
                    case "open":
                        if (channel.Status == "ACTIVE")
                        {
                            _logger.LogWarning("Attempting to open already active channel {ChannelId}", channel.Id);
                        }
                        else
                        {
                            channel.Status = "ACTIVE";
                            _logger.LogInformation("Channel {ChannelId} opened", channel.Id);
                        }
                        break;
                    case "close":
                        if (channel.Status == "CLOSED")
                        {
                            _logger.LogWarning("Attempting to close already closed channel {ChannelId}", channel.Id);
                        }
                        else
                        {
                            channel.Status = "CLOSED";
                            _logger.LogInformation("Channel {ChannelId} closed", channel.Id);
                        }
                        break;
                    case "suspend":
                        channel.Status = "SUSPENDED";
                        _logger.LogInformation("Channel {ChannelId} suspended", channel.Id);
                        break;
                    case "resume":
                        channel.Status = "ACTIVE";
                        _logger.LogInformation("Channel {ChannelId} resumed", channel.Id);
                        break;
                }
            }

            return await GetAllChannelsAsync();
        }

        public async Task<List<ChannelInfo>> DeleteChannelAsync(DeleteChannelRequest request)
        {
            _logger.LogTrace("Deleting channels");

            if (request.DeleteAll)
            {
                var count = _channels.Count;
                _channels.Clear();
                _logger.LogInformation("Deleted all {Count} channels", count);
            }
            else if (request.ChannelId.HasValue)
            {
                if (_channels.TryRemove(request.ChannelId.Value, out var channel))
                {
                    _logger.LogInformation("Deleted channel {ChannelId}", request.ChannelId.Value);
                }
                else
                {
                    _logger.LogError("Channel {ChannelId} not found for deletion", request.ChannelId.Value);
                }
            }
            else if (request.State?.ToUpper() == "CLOSED")
            {
                var closedChannels = _channels
                    .Where(pair => pair.Value.Status.ToUpper() == "CLOSED")
                    .Select(pair => pair.Key)
                    .ToList();

                foreach (var id in closedChannels)
                {
                    _channels.TryRemove(id, out _);
                }

                _logger.LogInformation("Deleted {Count} CLOSED channels", closedChannels.Count);
            }

            return await GetAllChannelsAsync();
        }

        public async Task<QueueOperationResponse> ExecuteQueueOperationAsync(QueueOperationRequest request)
        {
            _logger.LogTrace("Executing queue operation {Operation} on channel {ChannelId}",
                request.Operation, request.ChannelId);

            if (!_channels.TryGetValue(request.ChannelId, out var channel))
            {
                _logger.LogError("Channel {ChannelId} not found for queue operation", request.ChannelId);
                return new QueueOperationResponse
                {
                    Success = false,
                    Message = "Channel not found"
                };
            }

            try
            {
                switch (request.Operation.ToLower())
                {
                    case "enqueue":
                        var enqueueResult = await channel.EnqueueAsync(request.Data, _settings.WaitEnqueue);
                        if (!enqueueResult)
                        {
                            _logger.LogWarning("Enqueue operation timed out after {Timeout} seconds", _settings.WaitEnqueue);
                            return new QueueOperationResponse
                            {
                                Success = false,
                                Message = "Enqueue timeout"
                            };
                        }
                        _logger.LogDebug("Enqueued data to channel {ChannelId}", request.ChannelId);
                        return new QueueOperationResponse { Success = true, Message = "Enqueued successfully" };

                    case "dequeue":
                        var dequeueResult = await channel.DequeueAsync();
                        _logger.LogDebug("Dequeued data from channel {ChannelId}", request.ChannelId);
                        return new QueueOperationResponse
                        {
                            Success = true,
                            Data = dequeueResult,
                            Message = "Dequeued successfully"
                        };

                    case "peek":
                        var peekResult = await channel.PeekAsync();
                        _logger.LogDebug("Peeked data from channel {ChannelId}", request.ChannelId);
                        return new QueueOperationResponse
                        {
                            Success = true,
                            Data = peekResult,
                            Message = "Peeked successfully"
                        };

                    default:
                        _logger.LogError("Unknown queue operation {Operation}", request.Operation);
                        return new QueueOperationResponse
                        {
                            Success = false,
                            Message = "Unknown operation"
                        };
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Error executing queue operation {Operation} on channel {ChannelId}",
                    request.Operation, request.ChannelId);
                return new QueueOperationResponse
                {
                    Success = false,
                    Message = ex.Message
                };
            }
        }
    }

    public class ChannelWrapper
    {
        private readonly Channel<object> _channel;
        private readonly Queue<object> _peekQueue = new();
        private readonly object _lockObject = new();

        public Guid Id { get; }
        public string Name { get; }
        public string Status { get; set; } = "ACTIVE";
        public DateTime Created { get; } = DateTime.UtcNow;

        public ChannelWrapper(Guid id, string name, int capacity)
        {
            Id = id;
            Name = name;
            var options = new BoundedChannelOptions(capacity)
            {
                FullMode = BoundedChannelFullMode.Wait,
                SingleReader = false,
                SingleWriter = false
            };
            _channel = Channel.CreateBounded<object>(options);
        }

        public async Task<bool> EnqueueAsync(object? data, int timeoutSeconds)
        {
            if (Status == "CLOSED") return false;

            using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(timeoutSeconds));
            try
            {
                await _channel.Writer.WriteAsync(data ?? new object(), cts.Token);
                return true;
            }
            catch (OperationCanceledException)
            {
                return false;
            }
        }

        public async Task<object?> DequeueAsync()
        {
            lock (_lockObject)
            {
                if (_peekQueue.Count > 0)
                {
                    return _peekQueue.Dequeue();
                }
            }

            if (await _channel.Reader.WaitToReadAsync())
            {
                if (_channel.Reader.TryRead(out var item))
                {
                    return item;
                }
            }
            return null;
        }

        public async Task<object?> PeekAsync()
        {
            lock (_lockObject)
            {
                if (_peekQueue.Count > 0)
                {
                    return _peekQueue.Peek();
                }
            }

            if (await _channel.Reader.WaitToReadAsync())
            {
                if (_channel.Reader.TryRead(out var item))
                {
                    lock (_lockObject)
                    {
                        _peekQueue.Enqueue(item);
                        return item;
                    }
                }
            }
            return null;
        }

        public int GetCount()
        {
            lock (_lockObject)
            {
                return _peekQueue.Count;
            }
        }
    }
}