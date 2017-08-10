classdef CommandVersionUnitTest < BaseUnitTest
  % MexcppclassMexTest Test if mex file exist under directory
  %   Simple test if mex file exist and is executable by matlab
  
  properties 
    CommandName
    CommandResult 
  end
  
  methods 
      function obj = CommandVersionUnitTest() 
        obj = obj@BaseUnitTest();
        obj.CommandName = 'version'; 
      end
      function obj = PostSetup(obj) 
        obj.CommandResult = {obj.MexVersion, ... 
            obj.MexVersionMajor, obj.MexVersionMinor, ...
            obj.MexVersionPatch, obj.MexVersionTweak};
      end
  end
  
  methods (Test)

    function testIfCommandExist(testCase)
      % check if 'commands' command exist

      BaseUnitTest.forceClearMex(testCase.MexName);
      try
        testCase.verifyWarningFree(...
            @()testCase.MexFunction(testCase.CommandName));
      catch mx
        testCase.verifyFail(['warning/error : ' ... 
            BaseUnitTest.mxToText(mx) ' should not be issued']);
      end
    end
        
    function testResultOfCommandsCommand(testCase) 
        % check if result is correct 
        
        BaseUnitTest.forceClearMex(testCase.MexName); 
        testCase.MexFunction(testCase.CommandName);
        name = ans; 
        testCase.verifyEqual({name}, ....
            testCase.CommandResult(1), 'ans is wrong');
        
        [name] = ... 
            testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual({name}, ....
            testCase.CommandResult(1), 'argout(1) is wrong');
        
        [name, major] = ... 
            testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual({name major}, ....
            testCase.CommandResult(1:2), 'some of argout(1:2) are wrong');
        
        [name, major, minor] = ... 
            testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual({name major minor}, ....
            testCase.CommandResult(1:3), 'some of argout(1:3) are wrong');
        
        [name, major, minor, patch] = ... 
            testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual({name major minor patch}, ....
            testCase.CommandResult(1:4), 'some of argout(1:4) are wrong');
        
        [name, major, minor, patch, tweak] = ... 
            testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual({name major minor patch tweak}, ....
            testCase.CommandResult, 'some of argout(1:5) are wrong');
    end
    
    function testNoOutputs(testCase)
      % test if command can be call without any output arguments
      
      BaseUnitTest.forceClearMex(testCase.MexName);
      try
        testCase.verifyWarningFree(...
            @()testCase.MexFunction(testCase.CommandName));
      catch mx
        testCase.verifyTrue(['error should not be thrown : ' ... 
            BaseUnitTest.mxToText(mx)]);
      end
    end
    
     function testCheckIfCaseIsIgnored(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName); 
        try 
            testCase.verifyWarningFree(...
                @()testCase.MexFunction(upper(testCase.CommandName)));
        catch mx
            testCase.verifyFail(['Case should be ignored during ' ... 
                'function call; error: ' mxToText(mx)]);
        end 
        result_upper = testCase.MexFunction(upper(testCase.CommandName)); 
        result_lower = testCase.MexFunction(lower(testCase.CommandName)); 
        testCase.verifyEqual(result_upper, testCase.CommandResult{1}); 
        testCase.verifyEqual(result_lower, testCase.CommandResult{1}); 
     end
    
    function testErrorOnToManyInputs(testCase)
        % test if error is thrown when to many inputs are provided 
        
        BaseUnitTest.forceClearMex(testCase.MexName); 
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, 'not used'), ...
            [testCase.MexName ':' ...
                testCase.CommandName ':toManyInputs'], ... 
            'Wrong or no exception passed'); 
    end
    
    function testErrorOnToManyOutputs(testCase) 
        % test if error is thrown when to many outputs are provided 
        
        function commandCall()
            [a1, a2, a3, a4, a5, a6] = ... 
                testCase.MexFunction(testCase.CommandName); 
        end
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()commandCall, ...
            [testCase.MexName ':' testCase.CommandName ':toManyOutputs']); 
    end

  end
end
