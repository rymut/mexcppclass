classdef GeneralUnitTest < BaseUnitTest
  % MexBuildUnitTest Basic test checking if mex exists
  %   Mex must be placed inside matlab search path

  methods (Test)
     
      function testErrorOnInvalidCommand(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()testCase.MexFunction('???'), ...
            [testCase.MexName ':invalidCommand']); 
      end
    function testMexFileExist(testCase)
      % Check if mex file exist in current path

      testCase.verifyTrue(exist(testCase.MexName) == 3);
    end
  end

end
