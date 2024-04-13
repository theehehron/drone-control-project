clear; close all; clc;

tolerance = 0.01;
order_of_mag = .001;
for mags = 1:6
    for testseq = 1:50
        A = rand(3, 3)*order_of_mag;
        B = rand(3, 3)*order_of_mag;
        
        C1 = A*B;
        C2 = matrix_multiply_3x3(A, B);
        
        for i = 1:3
            for j = 1:3
                if (C1(i, j) - C2(i, j))/C1(i, j) > tolerance
                    warning(strcat("test failed at order of mag: ", num2str(order_of_mag)));
                    disp(C1-C2)
                    error_percent = min(min(C1(i, j)))/max(max(C1(i, j) - C2(i, j)));
                    disp(strcat("C1(i, j) is: ", num2str(C1(i, j))))
                    disp(strcat("error is: ", num2str(C1(i, j)-C2(i, j))))
                    disp(strcat("Error is ", num2str(error_percent), "%"))
                end
            end
        end
    end
    order_of_mag = order_of_mag*10;
end