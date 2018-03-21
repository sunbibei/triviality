close all; clear; clc;

EXPER_DIR='experiment1/Output';
SCHS={'Fc', 'Fe', 'Id', 'RR', 'SJ'};
% SCHS={'Fe'};
SIZE = 20;

% the means
util = zeros(length(SCHS), SIZE);
ress = zeros(length(SCHS), SIZE);
wait = zeros(length(SCHS), SIZE);
turn = zeros(length(SCHS), SIZE);
% the 'Fe' and 'RR'
util_Fe = zeros(SIZE, 10);
ress_Fe = zeros(SIZE, 10);
wait_Fe = zeros(SIZE, 10);
turn_Fe = zeros(SIZE, 10);
util_RR = zeros(SIZE, 10);
ress_RR = zeros(SIZE, 10);
wait_RR = zeros(SIZE, 10);
turn_RR = zeros(SIZE, 10);
% the 'SJF'
util_Sj = zeros(SIZE, 5, 4);
ress_Sj = zeros(SIZE, 5, 4);
wait_Sj = zeros(SIZE, 5, 4);
turn_Sj = zeros(SIZE, 5, 4);
for s_idx = 1:1:length(SCHS)
    sch = SCHS(s_idx);
    if isequal('Fe', char(sch))
        for idx = 1:1:SIZE
            for itr = 1:10
                TMP = importdata([EXPER_DIR '/output_' char(sch) '_'...
                    num2str(itr) '_' num2str(idx) '.out']);
                data= TMP.data;
                util_Fe(idx, itr) = data(end, end-1)/data(end, end-2);
                ress_Fe(idx, itr) = mean(data(1:end-1,end));
                wait_Fe(idx, itr) = mean(data(1:end-1,end-1));
                turn_Fe(idx, itr) = mean(data(1:end-1,end-2));
            end
            util(s_idx, idx) = mean(util_Fe(idx, :));
            ress(s_idx, idx) = mean(ress_Fe(idx, :));
            wait(s_idx, idx) = mean(wait_Fe(idx, :));
            turn(s_idx, idx) = mean(turn_Fe(idx, :));
        end
    elseif isequal('RR', char(sch))
        for idx = 1:1:SIZE
            for itr = 1:10
                TMP = importdata([EXPER_DIR '/output_' char(sch) '_'...
                    num2str(itr) '_' num2str(idx) '.out']);
                data= TMP.data;
                util_RR(idx, itr) = data(end, end-1)/data(end, end-2);
                ress_RR(idx, itr) = mean(data(1:end-1,end));
                wait_RR(idx, itr) = mean(data(1:end-1,end-1));
                turn_RR(idx, itr) = mean(data(1:end-1,end-2));
            end
            util(s_idx, idx) = mean(util_RR(idx, :));
            ress(s_idx, idx) = mean(ress_RR(idx, :));
            wait(s_idx, idx) = mean(wait_RR(idx, :));
            turn(s_idx, idx) = mean(turn_RR(idx, :));
        end
    elseif isequal('SJ', char(sch))
        for idx = 1:1:SIZE
            for tau = 1:5
                for alpha = 0:3
                    TMP = importdata([EXPER_DIR '/output_' char(sch) '_'...
                        num2str(tau) '_' num2str(alpha) '_' num2str(idx) '.out']);
                    data= TMP.data;
                    util_Sj(idx, tau, alpha+1) = data(end, end-1)/data(end, end-2);
                    ress_Sj(idx, tau, alpha+1) = mean(data(1:end-1,end));
                    wait_Sj(idx, tau, alpha+1) = mean(data(1:end-1,end-1));
                    turn_Sj(idx, tau, alpha+1) = mean(data(1:end-1,end-2));
                end
            end
            util(s_idx, idx) = mean(mean(util_Sj(idx,:,:)));
            ress(s_idx, idx) = mean(mean(ress_Sj(idx,:,:)));
            wait(s_idx, idx) = mean(mean(wait_Sj(idx,:,:)));
            turn(s_idx, idx) = mean(mean(turn_Sj(idx,:,:)));
        end
    else
        for idx = 1:1:SIZE
            TMP = importdata([EXPER_DIR '/output_' char(sch) '_'...
                    num2str(idx) '.out']);
            data= TMP.data;
            util(s_idx, idx) = data(end, end-1)/data(end, end-2);
            ress(s_idx, idx) = mean(data(1:end-1,end));
            wait(s_idx, idx) = mean(data(1:end-1,end-1));
            turn(s_idx, idx) = mean(data(1:end-1,end-2));
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% All of the Schedulers
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
ylabel('CPU Utilisation');
xlabel('The Number of Process');
for s_idx = 1:1:length(SCHS)
    plot(5:5:100, util(s_idx,:))
end
legend('FCFS', 'FbRR', 'ISJF', 'RR', 'SJF');

figure; hold on
ylabel('Response Time');
xlabel('The Number of Process');
for s_idx = 1:1:length(SCHS)
    plot(5:5:100, ress(s_idx,:))
end
legend('FCFS', 'FbRR', 'ISJF', 'RR', 'SJF');

figure; hold on
ylabel('Waiting Time');
xlabel('The Number of Process');
for s_idx = 1:1:length(SCHS)
    plot(5:5:100, wait(s_idx,:))
end
legend('FCFS', 'FbRR', 'ISJF', 'RR', 'SJF');

figure; hold on
ylabel('Turnaround Time');
xlabel('The Number of Process');
for s_idx = 1:1:length(SCHS)
    plot(5:5:100, turn(s_idx,:))
end
legend('FCFS', 'FbRR', 'ISJF', 'RR', 'SJF');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The FeedbackRR Scheduler Order By Process
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('FeedbackRRScheduler');
ylabel('CPU Utilisation');
xlabel('The Number of Process');
for idx = 1:1:size(util_Fe, 2)
    plot(5:5:100, util_Fe(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('FeedbackRRScheduler');
ylabel('Response Time');
xlabel('The Number of Process');
for idx = 1:1:size(ress_Fe, 2)
    plot(5:5:100, ress_Fe(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('FeedbackRRScheduler');
ylabel('Waiting Time');
xlabel('The Number of Process');
for idx = 1:1:size(wait_Fe, 2)
    plot(5:5:100, wait_Fe(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('FeedbackRRScheduler');
ylabel('Turnaround Time');
xlabel('The Number of Process');
for idx = 1:1:size(turn_Fe, 2)
    plot(5:5:100, turn_Fe(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The FeedbackRR Scheduler Order By Time Quantum
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('FeedbackRRScheduler');
ylabel('CPU Utilisation');
xlabel('Time Quantum');
plot(20:20:200, mean(util_Fe, 1))

figure; hold on
title('FeedbackRRScheduler');
ylabel('Response Time');
xlabel('Time Quantum');
plot(20:20:200, mean(ress_Fe, 1))

figure; hold on
title('FeedbackRRScheduler');
ylabel('Waiting Time');
xlabel('Time Quantum');
plot(20:20:200, mean(wait_Fe, 1))

figure; hold on
title('FeedbackRRScheduler');
ylabel('Turnaround Time');
xlabel('Time Quantum');
plot(20:20:200, mean(turn_Fe, 1))
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The RRScheduler Scheduler Order By Process
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('RRScheduler');
ylabel('CPU Utilisation');
xlabel('The Number of Process');
for idx = 1:1:size(util_RR, 2)
    plot(5:5:100, util_RR(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('RRScheduler');
ylabel('Response Time');
xlabel('The Number of Process');
for idx = 1:1:size(ress_RR, 2)
    plot(5:5:100, ress_RR(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('RRScheduler');
ylabel('Waiting Time');
xlabel('The Number of Process');
for idx = 1:1:size(wait_RR, 2)
    plot(5:5:100, wait_RR(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

figure; hold on
title('RRScheduler');
ylabel('Turnaround Time');
xlabel('The Number of Process');
for idx = 1:1:size(turn_RR, 2)
    plot(5:5:100, turn_RR(:,idx))
end
legend('20', '40', '60', '80', '100', '120', '140', '160', '180', '200');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The FeedbackRR Scheduler Order By Time Quantum
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('RRScheduler');
ylabel('CPU Utilisation');
xlabel('Time Quantum');
plot(20:20:200, mean(util_RR, 1))

figure; hold on
title('RRScheduler');
ylabel('Response Time');
xlabel('Time Quantum');
plot(20:20:200, mean(ress_RR, 1))

figure; hold on
title('RRScheduler');
ylabel('Waiting Time');
xlabel('Time Quantum');
plot(20:20:200, mean(wait_RR, 1))

figure; hold on
title('RRScheduler');
ylabel('Turnaround Time');
xlabel('Time Quantum');
plot(20:20:200, mean(turn_RR, 1))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The SJFScheduler Scheduler Order by Process and by tau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for tau = 1:5
    figure; hold on
    title(['SJFScheduler(tau=' num2str(tau*20) ')']);
    ylabel('CPU Utilisation');
    xlabel('The Number of Process');
    for idx = 1:1:size(util_Sj, 3)
        plot(5:5:100, util_Sj(:, tau, idx))
    end
    legend('.3', '.5', '.7', '.9');
end

for tau = 1:5
    figure; hold on
    title(['SJFScheduler(tau=' num2str(tau*20) ')']);
    ylabel('Response Time');
    xlabel('The Number of Process');
    for idx = 1:1:size(ress_Sj, 3)
        plot(5:5:100, ress_Sj(:, tau, idx))
    end
    legend('.3', '.5', '.7', '.9');
end

for tau = 1:5
    figure; hold on
    title(['SJFScheduler(tau=' num2str(tau*20) ')']);
    ylabel('Waiting Time');
    xlabel('The Number of Process');
    for idx = 1:1:size(wait_Sj, 3)
        plot(5:5:100, wait_Sj(:, tau, idx))
    end
    legend('.3', '.5', '.7', '.9');
end

for tau = 1:5
    figure; hold on
    title(['SJFScheduler(tau=' num2str(tau*20) ')']);
    ylabel('Turnaround Time');
    xlabel('The Number of Process');
    for idx = 1:1:size(turn_Sj, 3)
        plot(5:5:100, turn_Sj(:, tau, idx))
    end
    legend('.3', '.5', '.7', '.9');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The SJFScheduler Scheduler Order by Process and by alpha
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for alpha = 1:4
    figure; hold on
    title(['SJFScheduler(alpha=' num2str((alpha-1)*0.2+0.3) ')']);
    ylabel('CPU Utilisation');
    xlabel('The Number of Process');
    for tau = 1:1:size(util_Sj, 2)
        plot(5:5:100, util_Sj(:, tau, alpha))
    end
    legend('20', '40', '60', '80', '100');
end

for alpha = 1:4
    figure; hold on
    title(['SJFScheduler(alpha=' num2str((alpha-1)*0.2+0.3) ')']);
    ylabel('Response Time');
    xlabel('The Number of Process');
    for tau = 1:1:size(ress_Sj, 2)
        plot(5:5:100, ress_Sj(:, tau, alpha))
    end
    legend('20', '40', '60', '80', '100');
end

for alpha = 1:4
    figure; hold on
    title(['SJFScheduler(alpha=' num2str((alpha-1)*0.2+0.3) ')']);
    ylabel('Waiting Time');
    xlabel('The Number of Process');
    for tau = 1:1:size(wait_Sj, 2)
        plot(5:5:100, wait_Sj(:, tau, alpha))
    end
    legend('20', '40', '60', '80', '100');
end

for alpha = 1:4
    figure; hold on
    title(['SJFScheduler(alpha=' num2str((alpha-1)*0.2+0.3) ')']);
    ylabel('Turnaround Time');
    xlabel('The Number of Process');
    for tau = 1:1:size(turn_Sj, 2)
        plot(5:5:100, turn_Sj(:, tau, alpha))
    end
    legend('20', '40', '60', '80', '100');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The SJFScheduler Scheduler Order by tau and by alpha
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('SJFScheduler');
ylabel('CPU Utilisation');
xlabel('tau');
for alpha = 1:4
    plot(20:20:100, mean(util_Sj(:, :, alpha), 1)')
end
legend('.3', '.5', '.7', '.9');

figure; hold on
title('SJFScheduler');
ylabel('Response Time');
xlabel('tau');
for alpha = 1:4
    plot(20:20:100, mean(ress_Sj(:, :, alpha), 1)')
end
legend('.3', '.5', '.7', '.9');

figure; hold on
title('SJFScheduler');
ylabel('Waiting Time');
xlabel('tau');
for alpha = 1:4
    plot(20:20:100, mean(wait_Sj(:, :, alpha), 1)')
end
legend('.3', '.5', '.7', '.9');

figure; hold on
title('SJFScheduler');
ylabel('Turnaround Time');
xlabel('tau');
for alpha = 1:4
    plot(20:20:100, mean(turn_Sj(:, :, alpha), 1)')
end
legend('.3', '.5', '.7', '.9');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The SJFScheduler Scheduler Order by alpha and by tau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure; hold on
title('SJFScheduler');
ylabel('CPU Utilisation');
xlabel('alpha');
for tau = 1:5
    tmp=permute(util_Sj(:, tau, :),[1 3 2]);
    plot(0.3:0.2:0.9, mean(tmp, 1))
end
legend('20', '40', '60', '80', '100');

figure; hold on
title('SJFScheduler');
ylabel('Response Time');
xlabel('alpha');
for tau = 1:5
    tmp=permute(ress_Sj(:, tau, :),[1 3 2]);
    plot(0.3:0.2:0.9, mean(tmp, 1))
end
legend('20', '40', '60', '80', '100');

figure; hold on
title('SJFScheduler');
ylabel('Waiting Time');
xlabel('alpha');
for tau = 1:5
    tmp=permute(wait_Sj(:, tau, :),[1 3 2]);
    plot(0.3:0.2:0.9, mean(tmp, 1))
end
legend('20', '40', '60', '80', '100');

figure; hold on
title('SJFScheduler');
ylabel('Turnaround Time');
xlabel('alpha');
for tau = 1:5
    tmp=permute(turn_Sj(:, tau, :),[1 3 2]);
    plot(0.3:0.2:0.9, mean(tmp, 1))
end
legend('20', '40', '60', '80', '100');