clear; close all; clc;

tolerance = 0.01;
order_of_mag = .001;
test_count = 0;
fail_count = 0;
failed = 0;
for mags = 1:6
    for testseq = 1:50
        test_count = test_count + 1;
        A = 2*(rand(3, 3)-1/2)*order_of_mag;

        q1 = dcm2quat(A);
        q2 = dcm_to_quat(A);

        for i = 1:4
            if (q1(i) - q2(i))/q1(i) > tolerance
                warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                disp(q1)
                disp(q2)
                % fail_count = fail_count + 1;
                % error_percent = min(min(C1(i, j)))/max(max(C1(i, j) - C2(i, j)));
                % disp(strcat("C1(i, j) is: ", num2str(C1(i, j))))
                % disp(strcat("error is: ", num2str(C1(i, j)-C2(i, j))))
                % disp(strcat("Error is ", num2str(error_percent), "%"))
                failed = 1;
            end
        end
        fail_count = fail_count + failed;
        failed = 0;
    end
    order_of_mag = order_of_mag*10;
end
pass_count = test_count - fail_count;
pass_percent = round(pass_count/test_count*100, 2);
disp(strcat("Passed ", num2str(pass_count), " of ", num2str(test_count),...
            " tests (", num2str(pass_percent), "%)."))