require "fast_osc/version"

if ENV['FAST_OSC_USE_FALLBACK'] == "true"
  # optionally force pure Ruby version
  warn "FastOsc: choosing to use pure Ruby version"
  require "fast_osc/pure_ruby_fallback_encode.rb"
  require "fast_osc/pure_ruby_fallback_decode.rb"

  module FastOsc
    def self.encode_single_message(address, args=[])
      SonicPi::OSC::OscEncode.new.encode_single_message(address, args)
    end

    def self.encode_single_bundle(ts, address, args=[])
      SonicPi::OSC::OscEncode.new.encode_single_bundle(ts, address, args)
    end

    def self.encode_no_bundles(address, args=[])
      SonicPi::OSC::OscEncode.new.encode_single_message(address, args)
    end
    def self.decode_single_message(m)
      SonicPi::OSC::OscDecode.new.decode_single_message(m)
    end

    def self.decode_no_bundles(m)
      SonicPi::OSC::OscDecode.new.decode_single_message(m)
    end

    def self.decode(m)
      SonicPi::OSC::OscDecode.new.decode(m)
    end
  end
else
  begin
    require "fast_osc/fast_osc"
  
    module FastOsc
      # Your code goes here...
    end
  rescue LoadError
    warn "FastOsc: Failed to load the fast_osc c-extension, falling back to pure Ruby version"
    require "fast_osc/pure_ruby_fallback_encode.rb"
    require "fast_osc/pure_ruby_fallback_decode.rb"

    module FastOsc
      def self.encode_single_message(address, args=[])
        SonicPi::OSC::OscEncode.new.encode_single_message(address, args)
      end

      def self.encode_single_bundle(ts, address, args=[])
        SonicPi::OSC::OscEncode.new.encode_single_bundle(ts, address, args)
      end

      def self.encode_no_bundles(address, args=[])
        SonicPi::OSC::OscEncode.new.encode_single_message(address, args)
      end
      def self.decode_single_message(m)
        SonicPi::OSC::OscDecode.new.decode_single_message(m)
      end

      def self.decode_no_bundles(m)
        SonicPi::OSC::OscDecode.new.decode_single_message(m)
      end

      def self.decode(m)
        SonicPi::OSC::OscDecode.new.decode(m)
      end
    end
  end
end
