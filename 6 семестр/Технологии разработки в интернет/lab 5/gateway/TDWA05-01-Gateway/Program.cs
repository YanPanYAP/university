using Ocelot.DependencyInjection;
using Ocelot.Middleware;
using TDWA05_01_Gateway.LoadBalancers;

var builder = WebApplication.CreateBuilder(args);

builder.Configuration
    .SetBasePath(builder.Environment.ContentRootPath)
    .AddJsonFile("ocelot.json", optional: false, reloadOnChange: true);

builder.Services
    .AddOcelot(builder.Configuration)
    .AddCustomLoadBalancer((_, __, serviceDiscoveryProvider) =>
        new NickWeightedLoadBalancer(serviceDiscoveryProvider.GetAsync));

var app = builder.Build();

await app.UseOcelot();

app.Run();
