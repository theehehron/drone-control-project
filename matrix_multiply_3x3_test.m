clear; close all; clc;

tolerance = 0.01;
order_of_mag = .001;
test_count = 0;
fail_count = 0;
for mags = 1:6
    for testseq = 1:800
        test_count = test_count + 1;
        A = 2.*(rand(3, 3)-1/2)*order_of_mag;
        B = 2.*(rand(3, 3)-1/2)*order_of_mag;
        
        C1 = A*B;
        C2 = matrix_multiply_3x3(A, B);
        
        for i = 1:3
            for j = 1:3
                if (C1(i, j) - C2(i, j))/C1(i, j) > tolerance
                    warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                    disp(C1-C2)
                    error_percent = (C1(i, j) - C2(i, j))/C1(i, j)*100;
                    disp(strcat("C1(i, j) is: ", num2str(C1(i, j))))
                    disp(strcat("error is: ", num2str(C1(i, j)-C2(i, j))))
                    disp(strcat("Error is ", num2str(error_percent), "%"))
                    fail_count = fail_count + 1;
                end
            end
        end
    end
    order_of_mag = order_of_mag*10;
end
pass_count = test_count - fail_count;
pass_percent = round(pass_count/test_count*100, 2);
disp(strcat("Passed ", num2str(pass_count), " of ", num2str(test_count),...
            " tests (", num2str(pass_percent), "%)."))