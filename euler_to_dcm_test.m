clear; close all; clc;

tolerance = 0.01;
order_of_mag = .001;
test_count = 0;
fail_count = 0;
for mags = 1:6
    for testseq = 1:50
        test_count = test_count + 1;
        phi = 2*pi*(rand(1, 1)-1/2);
        theta = 2*pi*(rand(1, 1)-1/2);
        psi = 2*pi*(rand(1, 1)-1/2);

        C1 = angle2dcm(psi, theta, phi);
        C2 = euler_to_dcm([phi, theta, psi]);
        
        for i = 1:3
            for j = 1:3
                if (C1(i, j) - C2(i, j))/C1(i, j) > tolerance
                    warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                    disp(C1-C2)
                    error_percent = min(min(C1(i, j)))/max(max(C1(i, j) - C2(i, j)));
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