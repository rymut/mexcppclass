function [status] = run_unit_test(unittest, setup, store_path)
  % run_unit_test - simple wrapper cmake matlab test framework

  if ~exist('store_path', 'var')
    store_path = pwd;
  end
  function [ text ] = detailsText( detail )
  %DETAILSTEXT Summary of this function goes here
  %   Detailed explanation goes here
    text = '';
    if ~exist('detail', 'var') || ...
        isfield(detail, 'DiagnosticRecord') == 0
      text = 'no details';
    return;
    end

    for i=1:numel(detail.DiagnosticRecord)
        item = detail.DiagnosticRecord(i);
        if isprop( item, 'TestDiagnosticResults') == 0
            continue;
        end
        for j=1:numel(item.TestDiagnosticResults)
            res = item.TestDiagnosticResults(j);
            if isprop(res, 'DiagnosticText') == 0
                continue;
            end
            t = strsplit(res.DiagnosticText, '\n');
            if numel(t) ~= 0
                if numel(text)
                    text = [text '; '];
                end
                text = [text t{1}];
            end
        end
     end
  end



  try
    setup_unit_test_properties_handle = str2func(setup);
    suite = eval(unittest);
    suite = setup_unit_test_properties_handle(suite);
    result = run(suite);
    save(fullfile(store_path, [unittest '.mat']), 'result');
    rt = table(result);

    details = rt.Details;
    details = cellfun(@detailsText, details, 'UniformOutput', 0);
    rt.Details = details;

    sortrows(rt,'Passed');

    writetable(rt, fullfile(store_path, [unittest '.csv']), ...
        'QuoteStrings',true);
   	status = max([result(1,:).Failed]);
  catch mx
    getReport(mx)
  	status = 1;
  end
end
