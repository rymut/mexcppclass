classdef CommandCommandsUnitTest < BaseUnitTest
  % MexcppclassMexTest Test if mex file exist under directory
  %   Simple test if mex file exist and is executable by matlab
  
  properties 
    CommandName
    CommandResult 
  end
  
  methods 
      function obj = CommandCommandsUnitTest() 
        obj = obj@BaseUnitTest();
        obj.CommandName = 'commands'; 
        obj.CommandResult = {'call', 'clear', 'commands', 'create', ... 
            'destroy', 'handle', 'libpath', 'methods', 'objects', ... 
            'type', 'types', 'version'};
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
        testCase.verifyFail(['warning/error : ' getReport(mx) ... 
            ' should not be issued']);
      end
    end
        
    function testResultOfCommandsCommand(testCase) 
        BaseUnitTest.forceClearMex(testCase.MexName); 
        result = testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual(result, testCase.CommandResult); 
        testCase.MexFunction(testCase.CommandName);
        answer = ans; 
        testCase.verifyEqual(answer, testCase.CommandResult); 
    end
    
    function testNoOutputs(testCase)
      % test if command can be call without any output arguments
      BaseUnitTest.forceClearMex(testCase.MexName);
      try
        testCase.verifyWarningFree(...
            @()testCase.MexFunction(testCase.CommandName));
      catch mx
        testCase.verifyFail(['function call without outputs ' ...
            'should be possible; error ' getReport(mx)]);
      end
    end
    
    function testCheckIfCaseIsIgnored(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName); 
        try 
            testCase.verifyWarningFree(...
                @()testCase.MexFunction(upper(testCase.CommandName)));
        catch mx
            testCase.verifyFail(['Case should be ignored during ' ... 
                'function call; error: ' getReport(mx)]);
        end
        result_upper = testCase.MexFunction(upper(testCase.CommandName)); 
        result_lower = testCase.MexFunction(lower(testCase.CommandName)); 
        testCase.verifyEqual(result_upper, testCase.CommandResult); 
        testCase.verifyEqual(result_lower, testCase.CommandResult); 
    end
    
    function testErrorOnToManyInputs(testCase)
        % test if error is thrown when to many inputs are provided 
        
        BaseUnitTest.forceClearMex(testCase.MexName); 
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, 'not used'), ...
            [testCase.MexName ':' testCase.CommandName ':toManyInputs']); 
    end
    
    function testErrorOnToManyOutputs(testCase) 
        % test if error is thrown when to many outputs are provided 
        
        function commandCall()
            [a b] = testCase.MexFunction(testCase.CommandName); 
        end
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()commandCall, ...
            [testCase.MexName ':' testCase.CommandName ':toManyOutputs']); 
    end
 
  end
end
