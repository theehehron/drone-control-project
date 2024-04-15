clear; close all; clc;

tolerance = 0.01;
order_of_mag = .00001;
test_count = 0;
fail_count = 0;
failed = 0;
for mags = 1:6
    for testseq = 1:800
        test_count = test_count + 1;
        Cbv = 2*(rand(3, 3)-1/2)*order_of_mag;
        
        [psi, theta, phi]= dcm2angle(Cbv);
        eulers1 = [phi; theta; psi];

        eulers2 = dcm_to_euler(Cbv);
        
        for i = 1:3
            if (eulers1(i) - eulers2(i))/eulers1(i) > tolerance %&& (eulers1(i) + eulers2(i))/eulers1(i) > tolerance
                warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                disp(eulers1)
                disp(eulers2)
                error_percent = (eulers1(i) - eulers2(i))/eulers1(i)*100;
                disp(strcat("Error percent is ", num2str(error_percent), "%"))
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