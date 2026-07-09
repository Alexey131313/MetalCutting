#include "TestScenarios.h"

std::vector<TestScenario>
createTestScenarios()
{
    std::vector<TestScenario> tests;

    {
        TestScenario test;

        test.name =
            "Rotation works";

        test.request.sheet =
            {
                200,
                150
            };

        test.request.parts =
            {
                {1,150,200,1,true}
            };

        test.expectedProduced = 1;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Rotation forbidden";

        test.request.sheet =
            {
                200,
                150
            };

        test.request.parts =
            {
                {1,150,200,1,false}
            };

        test.expectedProduced = 0;
        test.expectedUnproduced = 1;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Part larger than sheet";

        test.request.sheet =
            {
                300,
                300
            };

        test.request.parts =
            {
                {1,500,100,1,true}
            };

        test.expectedProduced = 0;
        test.expectedUnproduced = 1;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Perfect fill";

        test.request.sheet =
            {
                200,
                200
            };

        test.request.parts =
            {
                {1,100,100,4,true}
            };

        test.expectedProduced = 4;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Single part";

        test.request.sheet =
            {
                500,
                500
            };

        test.request.parts =
            {
                {1,100,100,1,true}
            };

        test.expectedProduced = 1;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Empty request";

        test.request.sheet =
            {
                500,
                500
            };

        test.expectedProduced = 0;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Part equals sheet";

        test.request.sheet =
            {
                300,
                300
            };

        test.request.parts =
            {
                {1,300,300,1,true}
            };

        test.expectedProduced = 1;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Greedy trap";

        test.request.sheet =
            {
                300,
                200
            };

        test.request.parts =
            {
                {1,200,100,1,true},
                {2,100,100,2,true}
            };

        test.expectedProduced = 3;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Mixed parts";

        test.request.sheet =
            {
                286,
                300
            };

        test.request.parts =
            {
                {1,120,120,2,true},
                {2,80,80,2,true},
                {3,60,60,4,true}
            };

        test.expectedProduced = 8;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    {
        TestScenario test;

        test.name =
            "Stress test";

        test.request.sheet =
            {
                1000,
                1000
            };

        test.request.parts =
            {
                {1,150,150,10,true},
                {2,100,200,10,true},
                {3,80,80,20,true},
                {4,50,300,5,true}
            };

        test.expectedProduced = 45;
        test.expectedUnproduced = 0;

        tests.push_back(test);
    }

    return tests;
}