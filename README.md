# http-load-generator

A simple closed loop load generator for HTTP requests to test HTTP servers.

> The load generator only supports `HTTP/1.0` and `GET` requests.

## Usage

Set the following configuration values in `http_client.hh`:

- `HOST`: The host to send requests to.
- `URL`: The URL to send requests to.
- `PORT`: The port to send requests to.
- `SANITY_CHECK`: The expected response head.
- `OUTPUT:` The expected response body.
- `FAULT_EXIT:` 0 for tolerant, 1 for strict.

Then, set the following environment variables in `run_generator.sh`:

- `USER_COUNT:` The number of users to simulate.
- `THINK_TIME:` The think time between requests.
- `TEST_DURATION:` The duration of the test in seconds.
- `CPU:` The CPU to run the load generator on.

> CPU is the CPU number, not the CPU name. You can find the CPU number by running `lscpu`.

Finally, to run the load generator, use the following command:

```bash
bash ./run_generator.sh
```

> The use the loadgenerator on the same device as the HTTP server, use mutual exclusive CPUs. Use `taskset` to set the CPU affinity of the http server and the load generator.
