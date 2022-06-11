% Create Environment Interface
env = CartPoleDiscreteAction
obsInfo = getObservationInfo(env)
actInfo = getActionInfo(env)
rng(0);

% TODO:Create DQN Agent

dnn = [
    featureInputLayer(obsInfo.Dimension(1),'Normalization','none','Name','state')
    fullyConnectedLayer(24,'Name','CriticStateFC1')
    reluLayer('Name','CriticRelu1')
    fullyConnectedLayer(24, 'Name','CriticStateFC2')
    reluLayer('Name','CriticCommonRelu')
    fullyConnectedLayer(length(actInfo.Elements),'Name','output')];
dnn = dlnetwork(dnn);

figure
plot(layerGraph(dnn))

criticOpts = rlRepresentationOptions("LearnRate", 0.001, "GradientThreshold", 1);
critic = rlQValueRepresentation(dnn, obsInfo, actInfo, 'Observation', {'state'});

agentOpts = rlDQNAgentOptions(...
    'UseDoubleDQN',false, ...    
    'TargetSmoothFactor',1, ...
    'TargetUpdateFrequency',4, ...   
    'ExperienceBufferLength',100000, ...
    'DiscountFactor',0.99, ...
    'MiniBatchSize',256);
agent = rlDQNAgent(critic, agentOpts);

% TODO:Train Agent

trainOpts = rlTrainingOptions(...
    'MaxEpisodes',1000, ...
    'MaxStepsPerEpisode',500, ...
    'Verbose',false, ...
    'Plots','training-progress',...
    'StopTrainingCriteria','AverageReward',...
    'StopTrainingValue',480);

plot(env)

trainingStats = train(agent, env, trainOpts)

% TODO:Simulate DQN Agent

simOptions = rlSimulationOptions('MaxSteps', 500);
experience = sim(env, agent, simOptions)
totalReward = sum(experience.Reward)
