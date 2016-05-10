require 'test_helper'
require 'osc-ruby'
require 'date'

class FastOscTest < Minitest::Test
  def setup
    @path = "/thisisatest"
    @args = [1, 2.0, "baz"]
    @timestamp = Date.parse("1st Jan 1990").to_time

    @msg1 = OSC::Message.new(@path, *@args).encode
    @encoded_msg1 = @msg1.encode
  end

  def test_that_it_has_a_version_number
    refute_nil ::FastOsc::VERSION
  end

  def test_that_it_encodes_a_single_message
    msg = FastOsc.encode_single_message(@path, @args)

    assert msg == @encoded_msg1
  end

  def test_that_it_decodes_a_single_message
    path, args = FastOsc.decode_single_message(@encoded_msg1)

    assert path == @path
    assert args == @args
  end

  def test_that_it_encodes_a_single_bundle
    bundle1 = OSC::Bundle.new(@timestamp, @msg1).encode
    bundle2 = FastOsc.encode_single_bundle(@timestamp, @path, @args)

    assert_equal bundle1, bundle2
  end

  def test_that_it_encodes_a_single_bundle_with_fractional_time
    bundle1 = OSC::Bundle.new(@timestamp + 0.33, @msg1).encode
    bundle2 = FastOsc.encode_single_bundle(@timestamp + 0.33, @path, @args)

    assert_equal bundle1, bundle2
  end
end
