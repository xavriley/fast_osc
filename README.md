# FastOsc

`WARNING - Work in progress. Not working yet`

A Ruby wrapper around [rtosc](https://github.com/fundamental/rtosc/) to encode and decode OSC messages.

## Installation

Add this line to your application's Gemfile:

    gem 'fast_osc'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install fast_osc

This will only work on a mac at the moment as that's what the `librtosc.a` was compiled on.

## Usage

Planned API

```
FastOsc.serialise(["/aa", "foo", "bar"]) #=> "/aa\x00ss\x00\x00foo\x00bar\x00"
FastOsc.deserialise("/aa\x00ss\x00\x00foo\x00bar\x00") #=> ["foo", "bar"]
```

What currently works

```
[19:07:29] xavierriley:fast_osc git:(master*) $ ruby -r ./lib/fast_osc.bundle -e 'puts FastOsc.deserialize("/ab\x00ss\x00\x00foo\x00bar\x00").inspect'
["foo"]
```

It's a start...

## Development notes

https://gist.github.com/xavriley/507eff0a75d4552fa56e

## Contributing

1. Fork it ( http://github.com/<my-github-username>/fast_osc/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
