clear; close all; clc;

tolerance = 0.01;
order_of_mag = .00001;
test_count = 0;
euler_to_dcm_fail_count = 0;
failed = 0;
quat_fail_count = 0;


for mags = 1:6
    for testseq = 1:800
        test_count = test_count + 1;
        phi = 2*pi*(rand(1, 1)-1/2)*order_of_mag;
        theta = 2*pi*(rand(1, 1)-1/2)*order_of_mag;
        psi = 2*pi*(rand(1, 1)-1/2)*order_of_mag;

        % test of euler_to_dcm
        C1 = angle2dcm(psi, theta, phi);
        C2 = euler_to_dcm([phi, theta, psi]);

        % test of dcm_to_quat
        q1 = dcm2quat(C1);
        q2 = dcm_to_quat(C1);
        


        %% Checks for angle_2_dcm
        for i = 1:3
            for j = 1:3
                if (C1(i, j) - C2(i, j))/C1(i, j) > tolerance
                    warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                    disp(C1-C2)
                    error_percent = (C1(i, j) - C2(i, j))/C1(i, j)*100;
                    disp(strcat("C1(i, j) is: ", num2str(C1(i, j))))
                    disp(strcat("C2(i, j) is: ", num2str(C2(i, j))))
                    disp(strcat("error is: ", num2str(C1(i, j)-C2(i, j))))
                    disp(strcat("Error is ", num2str(error_percent), "%"))
                    euler_to_dcm_fail_count = euler_to_dcm_fail_count + 1;
                end
            end
        end


        
        %% Checks for dcm_to_quat
        for i = 1:4
            if (q1(i) - q2(i))/q1(i) > tolerance && (q1(i) + q2(i))/q1(i) > tolerance
                warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                disp(q1)
                disp(q2)
                % quat_fail_count = quat_fail_count + 1;
                % error_percent = min(min(C1(i, j)))/max(max(C1(i, j) - C2(i, j)));
                % disp(strcat("C1(i, j) is: ", num2str(C1(i, j))))
                % disp(strcat("error is: ", num2str(C1(i, j)-C2(i, j))))
                % disp(strcat("Error is ", num2str(error_percent), "%"))
                failed = 1;
            end
            
        end
        quat_fail_count = quat_fail_count + failed;
        failed = 0;
    end
    order_of_mag = order_of_mag*10;
end
euler_pass_count = test_count - euler_to_dcm_fail_count;
euler_pass_percent = round(euler_pass_count/test_count*100, 2);
disp(strcat("euler_to_dcm passed ", num2str(euler_pass_count), " of ", num2str(test_count),...
            " tests (", num2str(euler_pass_percent), "%)."))

quat_pass_count = test_count - quat_fail_count;
quat_pass_percent = round(quat_pass_count/test_count*100, 2);
disp(strcat("dcm_to_quat passed ", num2str(quat_pass_count), " of ", num2str(test_count),...
            " tests (", num2str(quat_pass_percent), "%)."))