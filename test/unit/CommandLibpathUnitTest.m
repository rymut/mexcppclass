classdef CommandLibpathUnitTest < BaseUnitTest
  % MexcppclassMexTest Test if mex file exist under directory
  %   Simple test if mex file exist and is executable by matlab

  properties
    CommandName
    CommandResult
  end

  methods
      function obj = CommandLibpathUnitTest()
        obj = obj@BaseUnitTest();
        obj.CommandName = 'libpath';
      end
      function obj = PostSetup(obj)
        obj.CommandResult = {fullfile('.', '/'), fullfile(fileparts(which('mexcppclass')), '/') };
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

    function testDefaultResult(testCase)
        % check if result is correct

        BaseUnitTest.forceClearMex(testCase.MexName);
        paths = testCase.MexFunction(testCase.CommandName);

        testCase.verifyEqual(paths, ....
            testCase.CommandResult, 'Ans is wrong');

        paths = testCase.MexFunction(testCase.CommandName);
        testCase.verifyEqual(paths, testCase.CommandResult);
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
        testCase.verifyEqual(result_upper, testCase.CommandResult);
        testCase.verifyEqual(result_lower, testCase.CommandResult);
     end

     function testErrorOnInvalidArguments(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, '?'), ...
            [testCase.MexName ':' ...
                testCase.CommandName ':invalidArguments'], ...
            'Wrong or no exception passed');
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, '?', '.'), ...
            [testCase.MexName ':' ...
                testCase.CommandName ':invalidArguments'], ...
            'Wrong or no exception passed');
     end
    function testAddOrDelErrorOnToManyInputs(testCase)
        % test if error is thrown when to many inputs are provided

        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, 'add', 'p', ...
                'not used'), ...
            [testCase.MexName ':' ...
                testCase.CommandName ':toManyInputs'], ...
            'Wrong or no exception passed');
        testCase.verifyError(...
            @()testCase.MexFunction(testCase.CommandName, 'del', 'p', ...
                'not used'), ...
            [testCase.MexName ':' ...
                testCase.CommandName ':toManyInputs'], ...
            'Wrong or no exception passed');
    end

    function testErrorOnToManyOutputs(testCase)
        % test if error is thrown when to many outputs are provided

        function commandCall()
            [a1, a2] = ...
                testCase.MexFunction(testCase.CommandName);
        end
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(...
            @()commandCall, ...
            [testCase.MexName ':' testCase.CommandName ':toManyOutputs']);
    end

    function testAddErrorOnNotEnoughInputs(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(@()testCase.MexFunction(...
            testCase.CommandName, 'add'), ...
            [testCase.MexName ':' testCase.CommandName ...
                ':notEnoughInputs']);
    end

    function testAddErrorOnToManyInputs(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(@()testCase.MexFunction(...
            testCase.CommandName, 'add', 'path', 'not used'), ...
            [testCase.MexName ':' testCase.CommandName ':toManyInputs']);
    end

    function testAddSubCommand(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        test_paths = strsplit(path, ';');
        paths = testCase.MexFunction(testCase.CommandName);
        pre_count = numel(paths);
        items = 0;
        for i=1:min(5, numel(test_paths))
            paths = testCase.MexFunction(testCase.CommandName, 'add', ...
                test_paths{i});
            testCase.verifyEqual(paths{end}, test_paths{i});
            items = items + 1;
            post_count = numel(paths);
        end
        testCase.verifyEqual(post_count - pre_count, items);
    end

    function testAddSubCommandIgnoreDuplicate(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        paths = testCase.MexFunction(testCase.CommandName);
        if numel(paths) == 0
            paths = strsplit(path, ';');
            paths = testCase.MexFunction(testCase.CommandName, 'add', ...
                paths(1));
        end
        pre_count = numel(paths);
        testCase.verifyGreaterThan(pre_count, 0);
        paths = testCase.MexFunction(testCase.CommandName, 'add', ...
            paths{1});
        post_count = numel(paths);
        testCase.verifyEqual(pre_count, post_count, ['duplication of ' ...
            'path should not be added']);
    end

    function testDelSubCommand(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        test_paths = strsplit(path, ';');
        paths = {};
        paths = testCase.MexFunction(testCase.CommandName);
        pre_count = numel(paths);
        for i=1:min(5, numel(test_paths))
            paths = testCase.MexFunction(testCase.CommandName, 'add', ...
                test_paths{i});
        end
        post_count = numel(paths);
        testCase.verifyGreaterThan(post_count - pre_count, 0);
        for i=1:min(5, numel(test_paths))
            paths = testCase.MexFunction(testCase.CommandName, 'del', ...
                test_paths{i});
            testCase.verifyEqual(...
                sum(...
                    cellfun(@(x)strcmp(x,test_paths{i}), ...
                        paths, 'UniformOutput', 1)), ...
                0);
        end
        testCase.verifyEqual(pre_count, numel(paths));
    end

    function testDelErrorOnNotEnoughInputs(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(@()testCase.MexFunction(...
            testCase.CommandName, 'del'), ...
            [testCase.MexName ':' testCase.CommandName ...
                ':notEnoughInputs']);
    end

    function testDelErrorOnToManyInputs(testCase)
        BaseUnitTest.forceClearMex(testCase.MexName);
        testCase.verifyError(@()testCase.MexFunction(...
            testCase.CommandName, 'del', 'path', 'not used'), ...
            [testCase.MexName ':' testCase.CommandName ':toManyInputs']);
    end
  end
end
