classdef BaseUnitTest < matlab.unittest.TestCase
  % BaseUnitTest Base class for unit testing of main compontent
  %

  properties
    MexName
    MexVersion
    MexVersionMajor
    MexVersionMinor
    MexVersionPatch
    MexVersionTweak
  end

  properties (Access = protected)
    MexFunction
  end

  methods
    function obj = set.MexName(obj, value)
      % Set Mex name and create function pointer for test use
      obj.MexName = value;
      obj.MexFunction = str2func(obj.MexName);
    end
    
    function delete(obj) 
        BaseUnitTest.forceClearMex(obj.MexName); 
    end
  end

  methods (Static)
      function text = mxToText(mx)
        text = ['id: ' mx.identifier '; msg: ' mx.message]; 
     end
    function forceClearMex(name)
      % Method used for clearing mex from memory
      if (mislocked(name) == true)
        munlock(name);
      end
      clear(name);
    end
  end

  methods
    function obj = BaseUnitTest()
       obj = obj@matlab.unittest.TestCase();
    end
  end

end
