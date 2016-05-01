# FastOsc

`WARNING - Work in progress. Probably not safe for production yet`

A Ruby wrapper around [rtosc](https://github.com/fundamental/rtosc/) to encode and decode OSC messages.

## Installation

Add this line to your application's Gemfile:

    gem 'fast_osc'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install fast_osc

This will only work on a mac at the moment as that's what the `librtosc.a` was compiled on.

## Is it fast?

Let's see...

Key:

* `fast_osc` - this gem
* `osc` - [`osc-ruby`](https://github.com/aberant/osc-ruby)
* `samsosc` - `OSC` classes from Sonic Pi (which are optimised pure Ruby based on `pack` and `unpack`)

### Encoding Benchmark

```
["/feeooblah", ["beans", 1, 2.0]]
Calculating -------------------------------------
            fast_osc    54.101k i/100ms
                 osc     7.688k i/100ms
             samsosc    21.406k i/100ms
-------------------------------------------------
            fast_osc    909.680k (±21.4%) i/s -      4.328M
                 osc     94.678k (±14.5%) i/s -    468.968k
             samsosc    271.908k (±19.3%) i/s -      1.327M
```

## Decoding Bencmark

```
["/feeooblah", ["beans", 1, 2.0]]
Calculating -------------------------------------
            fast_osc    91.434k i/100ms
             samsosc    22.095k i/100ms
             oscruby     3.522k i/100ms
-------------------------------------------------
            fast_osc      2.635M (±22.2%) i/s -     12.435M
             samsosc    264.614k (±16.1%) i/s -      1.304M
             oscruby     36.362k (±16.3%) i/s -    179.622k
```

Benchmark adapted from https://github.com/samaaron/sonic-pi/blob/master/app/server/sonicpi/test/performance/test_osc_perf.rb

I'll include a better test in the repo in time.

## Usage

```
>> FastOsc.serialize("/foo", ["baz", 1, 2.0])
=> "/foo\x00\x00\x00\x00,sif\x00\x00\x00\x00baz\x00\x00\x00\x00\x01@\x00\x00\x00"
>> res = _
>> FastOsc.deserialize(res)
=> ["baz", 1, 2.0]
```

## Still todo

* Implement more types
* return address with `deserialize` (doh!)
* add tests at the Ruby level (rtosc C code is already tested)
* figure out build process

## Development notes

    bundle install
    rake compile

https://gist.github.com/xavriley/507eff0a75d4552fa56e

## Contributing

1. Fork it ( http://github.com/<my-github-username>/fast_osc/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
