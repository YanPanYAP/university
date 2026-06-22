using Ocelot.LoadBalancer.LoadBalancers;
using Ocelot.Responses;
using Ocelot.Values;

namespace TDWA05_01_Gateway.LoadBalancers;

public sealed class NickWeightedLoadBalancer : ILoadBalancer
{
    private readonly Func<Task<List<Service>>> _services;
    private readonly object _lock = new();
    private int _rrCursor;

    public NickWeightedLoadBalancer(Func<Task<List<Service>>> services)
    {
        _services = services;
    }

    public string Type => nameof(NickWeightedLoadBalancer);

    public void Release(ServiceHostAndPort hostAndPort)
    {
    }

    public async Task<Response<ServiceHostAndPort>> LeaseAsync(HttpContext context)
    {
        var services = await _services.Invoke();

        if (services.Count == 0)
        {
            throw new InvalidOperationException("No downstream services available for load balancing.");
        }

        var x = services.Where(s => s.HostAndPort.DownstreamPort == 3101).ToList();
        var y = services.Where(s => s.HostAndPort.DownstreamPort == 3102).ToList();
        var z = services.Where(s => s.HostAndPort.DownstreamPort == 3103).ToList();

        var weighted = new List<Service>(10);
        if (x.Count > 0) weighted.AddRange(Enumerable.Repeat(x[0], 5));
        if (y.Count > 0) weighted.AddRange(Enumerable.Repeat(y[0], 3));
        if (z.Count > 0) weighted.AddRange(Enumerable.Repeat(z[0], 2));

        if (weighted.Count > 0)
        {
            lock (_lock)
            {
                _rrCursor %= weighted.Count;
                var selected = weighted[_rrCursor++];
                return new OkResponse<ServiceHostAndPort>(selected.HostAndPort);
            }
        }

        lock (_lock)
        {
            _rrCursor %= services.Count;
            var selected = services[_rrCursor++];
            return new OkResponse<ServiceHostAndPort>(selected.HostAndPort);
        }
    }
}
