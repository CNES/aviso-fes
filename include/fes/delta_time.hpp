// Copyright (c) 2026 CNES
//
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.
/// @file include/fes/delta_time.hpp
/// @brief Delta T (TT - UT1)
#pragma once
#include <array>
#include <cstddef>

#include "fes/detail/math.hpp"
#include "fes/numbers.hpp"

namespace fes {

/// @brief Delta T entry structure
struct DeltaTEntry {
  double mjd;      ///< Modified Julian Date (UTC) of the value
  double delta_t;  ///< Delta T in seconds
};

/// @brief Table generated dynamically from IERS (finals.all)
/// @note Calendar-month means of the daily values, each stamped with the mean
/// Modified Julian Date (UTC) of the days averaged. IERS values from 1973-01-02
/// to 2026-09-10, IERS Bulletin A predictions to 2027-09-18. leap-seconds.list
/// updated on 2026-07-06, expires on 2027-06-28.
/// SHA-256 of finals.all:
/// 6db7626247feb92f31197676b599762565d9aa863c70b21f5761db157c53c26b
/// SHA-256 of leap-seconds.list:
/// db5a895f16853b03bfc865e8d68f9fc8710ef1740e3400c701cd46a5bbbc3433
constexpr std::array<DeltaTEntry, 657> IERS_TABLE = {{
    {41698.5, 43.4228}, {41727.5, 43.5179}, {41757.0, 43.6180},
    {41787.5, 43.7250}, {41818.0, 43.8268}, {41848.5, 43.9163},
    {41879.0, 43.9929}, {41910.0, 44.0690}, {41940.5, 44.1527},
    {41971.0, 44.2443}, {42001.5, 44.3443}, {42032.0, 44.4395},
    {42063.0, 44.5236}, {42092.5, 44.6017}, {42122.0, 44.6888},
    {42152.5, 44.7867}, {42183.0, 44.8829}, {42213.5, 44.9637},
    {42244.0, 45.0271}, {42275.0, 45.0916}, {42305.5, 45.1654},
    {42336.0, 45.2500}, {42366.5, 45.3430}, {42397.0, 45.4327},
    {42428.0, 45.5173}, {42457.5, 45.6012}, {42487.0, 45.6881},
    {42517.5, 45.7808}, {42548.0, 45.8706}, {42578.5, 45.9480},
    {42609.0, 46.0106}, {42640.0, 46.0727}, {42670.5, 46.1433},
    {42701.0, 46.2290}, {42731.5, 46.3240}, {42762.0, 46.4123},
    {42793.0, 46.4984}, {42823.0, 46.5860}, {42853.0, 46.6783},
    {42883.5, 46.7777}, {42914.0, 46.8764}, {42944.5, 46.9623},
    {42975.0, 47.0329}, {43006.0, 47.1056}, {43036.5, 47.1863},
    {43067.0, 47.2869}, {43097.5, 47.3851}, {43128.0, 47.4751},
    {43159.0, 47.5620}, {43188.5, 47.6431}, {43218.0, 47.7294},
    {43248.5, 47.8270}, {43279.0, 47.9235}, {43309.5, 48.0019},
    {43340.0, 48.0630}, {43371.0, 48.1245}, {43401.5, 48.2001},
    {43432.0, 48.2925}, {43462.5, 48.3894}, {43493.0, 48.4838},
    {43524.0, 48.5826}, {43553.5, 48.6790}, {43583.0, 48.7816},
    {43613.5, 48.8851}, {43644.0, 48.9819}, {43674.5, 49.0666},
    {43705.0, 49.1291}, {43736.0, 49.1911}, {43766.5, 49.2656},
    {43797.0, 49.3535}, {43827.5, 49.4466}, {43858.0, 49.5393},
    {43889.0, 49.6318}, {43918.5, 49.7180}, {43948.0, 49.8045},
    {43978.5, 49.8995}, {44009.0, 49.9919}, {44039.5, 50.0679},
    {44070.0, 50.1328}, {44101.0, 50.1933}, {44131.5, 50.2602},
    {44162.0, 50.3395}, {44192.5, 50.4198}, {44223.0, 50.4979},
    {44254.0, 50.5761}, {44284.0, 50.6492}, {44314.0, 50.7240},
    {44344.5, 50.8047}, {44375.0, 50.8827}, {44405.5, 50.9482},
    {44436.0, 51.0017}, {44467.0, 51.0530}, {44497.5, 51.1157},
    {44528.0, 51.1902}, {44558.5, 51.2677}, {44589.0, 51.3422},
    {44620.0, 51.4161}, {44649.5, 51.4826}, {44679.0, 51.5580},
    {44709.5, 51.6391}, {44740.0, 51.7183}, {44770.5, 51.7865},
    {44801.0, 51.8315}, {44832.0, 51.8768}, {44862.5, 51.9270},
    {44893.0, 51.9947}, {44923.5, 52.0658}, {44954.0, 52.1326},
    {44985.0, 52.1997}, {45014.5, 52.2602}, {45044.0, 52.3289},
    {45074.5, 52.4057}, {45105.0, 52.4817}, {45135.5, 52.5463},
    {45166.0, 52.5955}, {45197.0, 52.6393}, {45227.5, 52.7000},
    {45258.0, 52.7697}, {45288.5, 52.8405}, {45319.0, 52.9165},
    {45350.0, 52.9970}, {45379.5, 53.0839}, {45409.0, 53.1708},
    {45439.5, 53.2599}, {45470.0, 53.3379}, {45500.5, 53.4057},
    {45531.0, 53.4551}, {45562.0, 53.5029}, {45592.5, 53.5562},
    {45623.0, 53.6174}, {45653.5, 53.6889}, {45684.0, 53.7551},
    {45715.0, 53.8128}, {45745.0, 53.8588}, {45775.0, 53.9124},
    {45805.5, 53.9730}, {45836.0, 54.0296}, {45866.5, 54.0704},
    {45897.0, 54.0964}, {45928.0, 54.1246}, {45958.5, 54.1658},
    {45989.0, 54.2166}, {46019.5, 54.2699}, {46050.0, 54.3190},
    {46081.0, 54.3671}, {46110.5, 54.4095}, {46140.0, 54.4601},
    {46170.5, 54.5179}, {46201.0, 54.5722}, {46231.5, 54.6164},
    {46262.0, 54.6438}, {46293.0, 54.6642}, {46323.5, 54.6949},
    {46354.0, 54.7434}, {46384.5, 54.7991}, {46415.0, 54.8503},
    {46446.0, 54.8922}, {46475.5, 54.9368}, {46505.0, 54.9772},
    {46535.5, 55.0226}, {46566.0, 55.0709}, {46596.5, 55.1008},
    {46627.0, 55.1222}, {46658.0, 55.1424}, {46688.5, 55.1699},
    {46719.0, 55.2155}, {46749.5, 55.2617}, {46780.0, 55.3019},
    {46811.0, 55.3416}, {46840.5, 55.3816}, {46870.0, 55.4335},
    {46900.5, 55.4853}, {46931.0, 55.5313}, {46961.5, 55.5685},
    {46992.0, 55.5918}, {47023.0, 55.6109}, {47053.5, 55.6441},
    {47084.0, 55.6909}, {47114.5, 55.7424}, {47145.0, 55.7927},
    {47176.0, 55.8409}, {47206.0, 55.8854}, {47236.0, 55.9402},
    {47266.5, 55.9945}, {47297.0, 56.0459}, {47327.5, 56.0847},
    {47358.0, 56.1016}, {47389.0, 56.1194}, {47419.5, 56.1427},
    {47450.0, 56.1824}, {47480.5, 56.2321}, {47511.0, 56.2800},
    {47542.0, 56.3182}, {47571.5, 56.3592}, {47601.0, 56.4037},
    {47631.5, 56.4529}, {47662.0, 56.5070}, {47692.5, 56.5528},
    {47723.0, 56.5824}, {47754.0, 56.6145}, {47784.5, 56.6510},
    {47815.0, 56.7003}, {47845.5, 56.7637}, {47876.0, 56.8273},
    {47907.0, 56.8815}, {47936.5, 56.9422}, {47966.0, 57.0080},
    {47996.5, 57.0799}, {48027.0, 57.1424}, {48057.5, 57.1974},
    {48088.0, 57.2405}, {48119.0, 57.2818}, {48149.5, 57.3347},
    {48180.0, 57.3978}, {48210.5, 57.4660}, {48241.0, 57.5329},
    {48272.0, 57.5988}, {48301.5, 57.6636}, {48331.0, 57.7320},
    {48361.5, 57.8039}, {48392.0, 57.8744}, {48422.5, 57.9343},
    {48453.0, 57.9772}, {48484.0, 58.0175}, {48514.5, 58.0721},
    {48545.0, 58.1351}, {48575.5, 58.2014}, {48606.0, 58.2728},
    {48637.0, 58.3437}, {48667.0, 58.4181}, {48697.0, 58.4949},
    {48727.5, 58.5808}, {48758.0, 58.6572}, {48788.5, 58.7167},
    {48819.0, 58.7611}, {48850.0, 58.8090}, {48880.5, 58.8666},
    {48911.0, 58.9322}, {48941.5, 59.0063}, {48972.0, 59.0822},
    {49003.0, 59.1601}, {49032.5, 59.2368}, {49062.0, 59.3152},
    {49092.5, 59.3996}, {49123.0, 59.4840}, {49153.5, 59.5541},
    {49184.0, 59.6080}, {49215.0, 59.6623}, {49245.5, 59.7236},
    {49276.0, 59.7965}, {49306.5, 59.8749}, {49337.0, 59.9463},
    {49368.0, 60.0195}, {49397.5, 60.0864}, {49427.0, 60.1620},
    {49457.5, 60.2402}, {49488.0, 60.3169}, {49518.5, 60.3777},
    {49549.0, 60.4208}, {49580.0, 60.4656}, {49610.5, 60.5229},
    {49641.0, 60.5947}, {49671.5, 60.6682}, {49702.0, 60.7428},
    {49733.0, 60.8244}, {49762.5, 60.9009}, {49792.0, 60.9822},
    {49822.5, 61.0673}, {49853.0, 61.1492}, {49883.5, 61.2182},
    {49914.0, 61.2681}, {49945.0, 61.3105}, {49975.5, 61.3684},
    {50006.0, 61.4389}, {50036.5, 61.5119}, {50067.0, 61.5888},
    {50098.0, 61.6559}, {50128.0, 61.7122}, {50158.0, 61.7783},
    {50188.5, 61.8475}, {50219.0, 61.9175}, {50249.5, 61.9741},
    {50280.0, 62.0145}, {50311.0, 62.0513}, {50341.5, 62.0919},
    {50372.0, 62.1486}, {50402.5, 62.2094}, {50433.0, 62.2653},
    {50464.0, 62.3236}, {50493.5, 62.3730}, {50523.0, 62.4358},
    {50553.5, 62.5118}, {50584.0, 62.5790}, {50614.5, 62.6371},
    {50645.0, 62.6717}, {50676.0, 62.7151}, {50706.5, 62.7631},
    {50737.0, 62.8238}, {50767.5, 62.8873}, {50798.0, 62.9405},
    {50829.0, 62.9918}, {50858.5, 63.0504}, {50888.0, 63.1121},
    {50918.5, 63.1739}, {50949.0, 63.2320}, {50979.5, 63.2738},
    {51010.0, 63.2900}, {51041.0, 63.3040}, {51071.5, 63.3271},
    {51102.0, 63.3645}, {51132.5, 63.4106}, {51163.0, 63.4508},
    {51194.0, 63.4811}, {51223.5, 63.5145}, {51253.0, 63.5476},
    {51283.5, 63.5881}, {51314.0, 63.6272}, {51344.5, 63.6565},
    {51375.0, 63.6693}, {51406.0, 63.6820}, {51436.5, 63.7028},
    {51467.0, 63.7309}, {51497.5, 63.7719}, {51528.0, 63.8097},
    {51559.0, 63.8402}, {51589.0, 63.8673}, {51619.0, 63.8949},
    {51649.5, 63.9233}, {51680.0, 63.9555}, {51710.5, 63.9758},
    {51741.0, 63.9829}, {51772.0, 63.9863}, {51802.5, 63.9996},
    {51833.0, 64.0216}, {51863.5, 64.0540}, {51894.0, 64.0807},
    {51925.0, 64.1000}, {51954.5, 64.1146}, {51984.0, 64.1442},
    {52014.5, 64.1713}, {52045.0, 64.1984}, {52075.5, 64.2096},
    {52106.0, 64.2100}, {52137.0, 64.2078}, {52167.5, 64.2164},
    {52198.0, 64.2352}, {52228.5, 64.2627}, {52259.0, 64.2863},
    {52290.0, 64.3101}, {52319.5, 64.3314}, {52349.0, 64.3588},
    {52379.5, 64.3827}, {52410.0, 64.4044}, {52440.5, 64.4138},
    {52471.0, 64.4146}, {52502.0, 64.4092}, {52532.5, 64.4136},
    {52563.0, 64.4250}, {52593.5, 64.4417}, {52624.0, 64.4620},
    {52655.0, 64.4820}, {52684.5, 64.4968}, {52714.0, 64.5153},
    {52744.5, 64.5375}, {52775.0, 64.5530}, {52805.5, 64.5572},
    {52836.0, 64.5438}, {52867.0, 64.5361}, {52897.5, 64.5371},
    {52928.0, 64.5471}, {52958.5, 64.5595}, {52989.0, 64.5684},
    {53020.0, 64.5806}, {53050.0, 64.5936}, {53080.0, 64.6079},
    {53110.5, 64.6303}, {53141.0, 64.6479}, {53171.5, 64.6547},
    {53202.0, 64.6454}, {53233.0, 64.6367}, {53263.5, 64.6366},
    {53294.0, 64.6462}, {53324.5, 64.6633}, {53355.0, 64.6813},
    {53386.0, 64.6968}, {53415.5, 64.7155}, {53445.0, 64.7460},
    {53475.5, 64.7681}, {53506.0, 64.7929}, {53536.5, 64.7998},
    {53567.0, 64.7916}, {53598.0, 64.7860}, {53628.5, 64.7869},
    {53659.0, 64.7999}, {53689.5, 64.8197}, {53720.0, 64.8379},
    {53751.0, 64.8512}, {53780.5, 64.8718}, {53810.0, 64.8995},
    {53840.5, 64.9307}, {53871.0, 64.9647}, {53901.5, 64.9848},
    {53932.0, 64.9976}, {53963.0, 65.0074}, {53993.5, 65.0244},
    {54024.0, 65.0581}, {54054.5, 65.0949}, {54085.0, 65.1285},
    {54116.0, 65.1622}, {54145.5, 65.2006}, {54175.0, 65.2307},
    {54205.5, 65.2706}, {54236.0, 65.3100}, {54266.5, 65.3355},
    {54297.0, 65.3456}, {54328.0, 65.3472}, {54358.5, 65.3575},
    {54389.0, 65.3831}, {54419.5, 65.4110}, {54450.0, 65.4431},
    {54481.0, 65.4704}, {54511.0, 65.5010}, {54541.0, 65.5291},
    {54571.5, 65.5624}, {54602.0, 65.5971}, {54632.5, 65.6205},
    {54663.0, 65.6331}, {54694.0, 65.6424}, {54724.5, 65.6602},
    {54755.0, 65.6904}, {54785.5, 65.7262}, {54816.0, 65.7635},
    {54847.0, 65.7911}, {54876.5, 65.8124}, {54906.0, 65.8406},
    {54936.5, 65.8775}, {54967.0, 65.9149}, {54997.5, 65.9423},
    {55028.0, 65.9506}, {55059.0, 65.9571}, {55089.5, 65.9730},
    {55120.0, 65.9990}, {55150.5, 66.0275}, {55181.0, 66.0560},
    {55212.0, 66.0822}, {55241.5, 66.1124}, {55271.0, 66.1488},
    {55301.5, 66.1858}, {55332.0, 66.2206}, {55362.5, 66.2412},
    {55393.0, 66.2378}, {55424.0, 66.2332}, {55454.5, 66.2396},
    {55485.0, 66.2599}, {55515.5, 66.2907}, {55546.0, 66.3148},
    {55577.0, 66.3313}, {55606.5, 66.3504}, {55636.0, 66.3776},
    {55666.5, 66.4123}, {55697.0, 66.4457}, {55727.5, 66.4710},
    {55758.0, 66.4757}, {55789.0, 66.4781}, {55819.5, 66.4907},
    {55850.0, 66.5215}, {55880.5, 66.5544}, {55911.0, 66.5849},
    {55942.0, 66.6185}, {55972.0, 66.6457}, {56002.0, 66.6729},
    {56032.5, 66.7122}, {56063.0, 66.7443}, {56093.5, 66.7665},
    {56124.0, 66.7712}, {56155.0, 66.7784}, {56185.5, 66.7951},
    {56216.0, 66.8241}, {56246.5, 66.8575}, {56277.0, 66.8940},
    {56308.0, 66.9240}, {56337.5, 66.9591}, {56367.0, 67.0000},
    {56397.5, 67.0484}, {56428.0, 67.0917}, {56458.5, 67.1172},
    {56489.0, 67.1280}, {56520.0, 67.1382}, {56550.5, 67.1577},
    {56581.0, 67.1898}, {56611.5, 67.2271}, {56642.0, 67.2629},
    {56673.0, 67.2968}, {56702.5, 67.3276}, {56732.0, 67.3666},
    {56762.5, 67.4084}, {56793.0, 67.4501}, {56823.5, 67.4765},
    {56854.0, 67.4929}, {56885.0, 67.5046}, {56915.5, 67.5228},
    {56946.0, 67.5528}, {56976.5, 67.5891}, {57007.0, 67.6247},
    {57038.0, 67.6586}, {57067.5, 67.6933}, {57097.0, 67.7329},
    {57127.5, 67.7795}, {57158.0, 67.8211}, {57188.5, 67.8504},
    {57219.0, 67.8728}, {57250.0, 67.8957}, {57280.5, 67.9311},
    {57311.0, 67.9773}, {57341.5, 68.0274}, {57372.0, 68.0759},
    {57403.0, 68.1306}, {57433.0, 68.1795}, {57463.0, 68.2348},
    {57493.5, 68.2939}, {57524.0, 68.3448}, {57554.5, 68.3849},
    {57585.0, 68.4034}, {57616.0, 68.4187}, {57646.5, 68.4446},
    {57677.0, 68.4837}, {57707.5, 68.5304}, {57738.0, 68.5743},
    {57769.0, 68.6119}, {57798.5, 68.6482}, {57828.0, 68.6887},
    {57858.5, 68.7360}, {57889.0, 68.7824}, {57919.5, 68.8151},
    {57950.0, 68.8299}, {57981.0, 68.8423}, {58011.5, 68.8572},
    {58042.0, 68.8837}, {58072.5, 68.9185}, {58103.0, 68.9526},
    {58134.0, 68.9764}, {58163.5, 69.0035}, {58193.0, 69.0327},
    {58223.5, 69.0642}, {58254.0, 69.0948}, {58284.5, 69.1133},
    {58315.0, 69.1132}, {58346.0, 69.1176}, {58376.5, 69.1267},
    {58407.0, 69.1495}, {58437.5, 69.1796}, {58468.0, 69.2078},
    {58499.0, 69.2304}, {58528.5, 69.2579}, {58558.0, 69.2867},
    {58588.5, 69.3188}, {58619.0, 69.3444}, {58649.5, 69.3576},
    {58680.0, 69.3521}, {58711.0, 69.3408}, {58741.5, 69.3373},
    {58772.0, 69.3379}, {58802.5, 69.3481}, {58833.0, 69.3572},
    {58864.0, 69.3666}, {58894.0, 69.3811}, {58924.0, 69.4004},
    {58954.5, 69.4200}, {58985.0, 69.4345}, {59015.5, 69.4337},
    {59046.0, 69.4078}, {59077.0, 69.3816}, {59107.5, 69.3621},
    {59138.0, 69.3563}, {59168.5, 69.3613}, {59199.0, 69.3625},
    {59230.0, 69.3559}, {59259.5, 69.3539}, {59289.0, 69.3556},
    {59319.5, 69.3616}, {59350.0, 69.3674}, {59380.5, 69.3620},
    {59411.0, 69.3379}, {59442.0, 69.3155}, {59472.5, 69.2961},
    {59503.0, 69.2891}, {59533.5, 69.2908}, {59564.0, 69.2922},
    {59595.0, 69.2939}, {59624.5, 69.2888}, {59654.0, 69.2850},
    {59684.5, 69.2821}, {59715.0, 69.2811}, {59745.5, 69.2683},
    {59776.0, 69.2385}, {59807.0, 69.2101}, {59837.5, 69.1929},
    {59868.0, 69.1899}, {59898.5, 69.2001}, {59929.0, 69.2027},
    {59960.0, 69.2011}, {59989.5, 69.1967}, {60019.0, 69.2041},
    {60049.5, 69.2142}, {60080.0, 69.2257}, {60110.5, 69.2263},
    {60141.0, 69.2095}, {60172.0, 69.1909}, {60202.5, 69.1757},
    {60233.0, 69.1709}, {60263.5, 69.1728}, {60294.0, 69.1732},
    {60325.0, 69.1771}, {60355.0, 69.1839}, {60385.0, 69.1921},
    {60415.5, 69.2009}, {60446.0, 69.2042}, {60476.5, 69.1987},
    {60507.0, 69.1717}, {60538.0, 69.1448}, {60568.5, 69.1274},
    {60599.0, 69.1267}, {60629.5, 69.1315}, {60660.0, 69.1357},
    {60691.0, 69.1391}, {60720.5, 69.1376}, {60750.0, 69.1415},
    {60780.5, 69.1497}, {60811.0, 69.1548}, {60841.5, 69.1502},
    {60872.0, 69.1296}, {60903.0, 69.1102}, {60933.5, 69.0950},
    {60964.0, 69.0905}, {60994.5, 69.0978}, {61025.0, 69.1075},
    {61056.0, 69.1114}, {61085.5, 69.1158}, {61115.0, 69.1241},
    {61145.5, 69.1411}, {61176.0, 69.1573}, {61206.5, 69.1704},
    {61237.0, 69.1722}, {61268.0, 69.1753}, {61298.5, 69.1909},
    {61329.0, 69.2217}, {61359.5, 69.2576}, {61390.0, 69.2836},
    {61421.0, 69.3000}, {61450.5, 69.3151}, {61480.0, 69.3372},
    {61510.5, 69.3560}, {61541.0, 69.3654}, {61571.5, 69.3619},
    {61602.0, 69.3438}, {61633.0, 69.3199}, {61657.5, 69.3023},
}};

namespace detail {

/// @brief Duration, in years, of the transitions that join consecutive
/// Espenak & Meeus polynomials (centred on their common boundary) and that
/// align the last polynomial with the first entry of the IERS table (ending
/// on that entry).
constexpr double kHandoverYears = 2.0;

/// @brief Duration, in days, of the end of the IERS table whose least-squares
/// trend is extrapolated beyond the table.
constexpr double kTrendWindowDays = 2.0 * 365.25;

/// @brief Year from which the extrapolated trend is blended into the Morrison
/// & Stephenson (2004) parabola.
constexpr double kLongTermBlendStart = 2050.0;

/// @brief Year from which the Morrison & Stephenson (2004) parabola is used
/// alone.
constexpr double kLongTermBlendEnd = 2150.0;

/// @brief Boundaries, in years, between the formulas of Espenak & Meeus
/// (2006) used before the IERS table.
constexpr std::array<double, 10> kEspenakMeeusBoundaries = {
    {-500.0, 500.0, 1600.0, 1700.0, 1800.0, 1860.0, 1900.0, 1920.0, 1941.0,
     1961.0}};

/// @brief Modified Julian Date of the Unix epoch (1970-01-01T00:00:00Z).
constexpr double kUnixEpochModifiedJulianDay =
    numbers::kUnixEpochJulianDay - numbers::kModifiedJulianEpoch;

/// @brief Convert Unix epoch to Modified Julian Date.
/// @param[in] epoch Seconds since 1970-01-01T00:00:00Z.
/// @return Modified Julian Date.
constexpr auto epoch_to_mjd(double epoch) noexcept -> double {
  return (epoch / static_cast<double>(numbers::kSecondsPerDay)) +
         kUnixEpochModifiedJulianDay;
}

/// @brief Convert Modified Julian Date to decimal year, counted as in
/// epoch_to_year.
/// @param[in] mjd Modified Julian Date.
/// @return Decimal year.
constexpr auto mjd_to_year(double mjd) noexcept -> double {
  double t = (mjd + numbers::kModifiedJulianEpoch -
              static_cast<double>(numbers::kJ2000JulianDay)) /
             static_cast<double>(numbers::kDaysPerCentury);
  return 2000.0 + (t * 100.0);
}

/// @brief Hermite interpolation between 0 and 1 with zero slope at both ends.
/// @param[in] edge0 Value of x where the result starts to increase from 0.
/// @param[in] edge1 Value of x where the result reaches 1.
/// @param[in] x The evaluation point.
/// @return 0 if x <= edge0, 1 if x >= edge1, a C1 transition in between.
constexpr auto smoothstep(double edge0, double edge1, double x) noexcept
    -> double {
  if (x <= edge0) {
    return 0.0;
  }
  if (x >= edge1) {
    return 1.0;
  }
  double u = (x - edge0) / (edge1 - edge0);
  return u * u * (3.0 - (2.0 * u));
}

/// @brief Binary search to find the lower bound by Modified Julian Date.
/// @tparam N The number of entries of the table.
/// @param[in] table The table, sorted by increasing date.
/// @param[in] mjd The Modified Julian Date to search for.
/// @return The index of the first entry with a date >= mjd, or N if none.
template <std::size_t N>
constexpr auto lower_bound_by_mjd(const std::array<DeltaTEntry, N>& table,
                                  double mjd) noexcept -> std::size_t {
  std::size_t first = 0;
  std::size_t count = N;
  while (count > 0) {
    auto step = count / 2;
    if (table[first + step].mjd < mjd) {
      first += step + 1;
      count -= step + 1;
    } else {
      count = step;
    }
  }
  return first;
}

/// @brief Linear interpolation between two table entries.
/// @param[in] p1 The lower bound entry.
/// @param[in] p2 The upper bound entry.
/// @param[in] mjd The target Modified Julian Date.
/// @return Interpolated delta T value.
constexpr auto interpolate(const DeltaTEntry& p1, const DeltaTEntry& p2,
                           double mjd) noexcept -> double {
  double fraction = (mjd - p1.mjd) / (p2.mjd - p1.mjd);
  return p1.delta_t + (fraction * (p2.delta_t - p1.delta_t));
}

/// @brief Morrison & Stephenson (2004) long-term parabola.
/// @param[in] year Decimal year.
/// @return Delta T in seconds.
/// @note Valid approximation for years far from modern era.
constexpr auto morrison_stephenson_parabola(double year) noexcept -> double {
  // Reference for long-term parabola
  constexpr double kMorrisonEpoch = 1820.0;
  // Parabolic coefficient (s/century²)
  constexpr double kMorrisonCoeff = 32.0;
  // Constant offset
  constexpr double kMorrisonOffset = -20.0;

  double t_cent = (year - kMorrisonEpoch) / 100.0;
  return kMorrisonOffset + (kMorrisonCoeff * t_cent * t_cent);
}

// The polynomials below are those of Espenak & Meeus (2006), "Five Millennium
// Canon of Solar Eclipses: -1999 to +3000", NASA/TP-2006-214141, as published
// on https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html. Each of them
// fits the historical record over its own interval only.

/// @brief Polynomial approximation for -500 to +500.
/// @param[in] year Decimal year (should be in range [-500, 500]).
/// @return Delta T in seconds.
constexpr auto polynomial_m500_500(double year) noexcept -> double {
  double u = year / 100.0;
  return math::horner(u, 10583.6, -1014.41, 33.78311, -5.952053, -0.1798452,
                      0.022174192, 0.0090316521);
}

/// @brief Polynomial approximation for +500 to +1600.
/// @param[in] year Decimal year (should be in range [500, 1600]).
/// @return Delta T in seconds.
constexpr auto polynomial_500_1600(double year) noexcept -> double {
  double u = (year - 1000.0) / 100.0;
  return math::horner(u, 1574.2, -556.01, 71.23472, 0.319781, -0.8503463,
                      -0.005050998, 0.0083572073);
}

/// @brief Polynomial approximation for 1600-1700 era.
/// @param[in] year Decimal year (should be in range [1600, 1700]).
/// @return Delta T in seconds.
constexpr auto polynomial_1600_1700(double year) noexcept -> double {
  double t = year - 1600.0;
  return math::horner(t, 120.0, -0.9808, -0.01532, 1.0 / 7129.0);
}

/// @brief Polynomial approximation for 1700-1800 era.
/// @param[in] year Decimal year (should be in range [1700, 1800]).
/// @return Delta T in seconds.
constexpr auto polynomial_1700_1800(double year) noexcept -> double {
  double t = year - 1700.0;
  return math::horner(t, 8.83, 0.1603, -0.0059285, 0.00013336,
                      -1.0 / 1174000.0);
}

/// @brief Polynomial approximation for 1800-1860 era.
/// @param[in] year Decimal year (should be in range [1800, 1860]).
/// @return Delta T in seconds.
constexpr auto polynomial_1800_1860(double year) noexcept -> double {
  double t = year - 1800.0;
  return math::horner(t, 13.72, -0.332447, 0.0068612, 0.0041116, -0.00037436,
                      0.0000121272, -0.0000001699, 0.000000000875);
}

/// @brief Polynomial approximation for 1860-1900 era.
/// @param[in] year Decimal year (should be in range [1860, 1900]).
/// @return Delta T in seconds.
constexpr auto polynomial_1860_1900(double year) noexcept -> double {
  double t = year - 1860.0;
  return math::horner(t, 7.62, 0.5737, -0.251754, 0.01680668, -0.0004473624,
                      1.0 / 233174.0);
}

/// @brief Polynomial approximation for 1900-1920 era.
/// @param[in] year Decimal year (should be in range [1900, 1920]).
/// @return Delta T in seconds.
constexpr auto polynomial_1900_1920(double year) noexcept -> double {
  double t = year - 1900.0;
  return math::horner(t, -2.79, 1.494119, -0.0598939, 0.0061966, -0.000197);
}

/// @brief Polynomial approximation for 1920-1941 era.
/// @param[in] year Decimal year (should be in range [1920, 1941]).
/// @return Delta T in seconds.
constexpr auto polynomial_1920_1941(double year) noexcept -> double {
  double t = year - 1920.0;
  return math::horner(t, 21.20, 0.84493, -0.076100, 0.0020936);
}

/// @brief Polynomial approximation for 1941-1961 era.
/// @param[in] year Decimal year (should be in range [1941, 1961]).
/// @return Delta T in seconds.
constexpr auto polynomial_1941_1961(double year) noexcept -> double {
  double t = year - 1950.0;
  return math::horner(t, 29.07, 0.407, -1.0 / 233.0, 1.0 / 2547.0);
}

/// @brief Polynomial approximation for 1961-1986 era.
/// @param[in] year Decimal year (should be in range [1961, 1986]).
/// @return Delta T in seconds.
constexpr auto polynomial_1961_1986(double year) noexcept -> double {
  double t = year - 1975.0;
  return math::horner(t, 45.45, 1.067, -1.0 / 260.0, -1.0 / 718.0);
}

/// @brief Evaluates one of the Espenak & Meeus (2006) formulas.
/// @param[in] index Index of the formula: 0 for the parabola used before
/// the first boundary of kEspenakMeeusBoundaries, i for the polynomial
/// starting at its (i - 1)-th boundary.
/// @param[in] year Decimal year.
/// @return Delta T in seconds.
constexpr auto espenak_meeus_formula(std::size_t index, double year) noexcept
    -> double {
  switch (index) {
    case 0:
      return morrison_stephenson_parabola(year);
    case 1:
      return polynomial_m500_500(year);
    case 2:
      return polynomial_500_1600(year);
    case 3:
      return polynomial_1600_1700(year);
    case 4:
      return polynomial_1700_1800(year);
    case 5:
      return polynomial_1800_1860(year);
    case 6:
      return polynomial_1860_1900(year);
    case 7:
      return polynomial_1900_1920(year);
    case 8:
      return polynomial_1920_1941(year);
    case 9:
      return polynomial_1941_1961(year);
    default:
      return polynomial_1961_1986(year);
  }
}

/// @brief Delta T from the Espenak & Meeus (2006) formulas.
///
/// Consecutive formulas differ by up to 0.25 s at their common boundary. Over
/// kHandoverYears centred on each boundary, the result moves from one formula
/// to the next with a smoothstep weight, which makes it continuous.
///
/// @param[in] year Decimal year.
/// @return Delta T in seconds.
constexpr auto espenak_meeus(double year) noexcept -> double {
  double half_width = 0.5 * kHandoverYears;
  // Index of the formula covering year
  std::size_t index = 0;
  while (index < kEspenakMeeusBoundaries.size() &&
         year >= kEspenakMeeusBoundaries[index]) {
    ++index;
  }
  // Boundary whose handover contains year, if any
  std::size_t boundary = 0;
  if (index > 0 && year < kEspenakMeeusBoundaries[index - 1] + half_width) {
    boundary = index - 1;
  } else if (index < kEspenakMeeusBoundaries.size() &&
             year >= kEspenakMeeusBoundaries[index] - half_width) {
    boundary = index;
  } else {
    return espenak_meeus_formula(index, year);
  }
  double center = kEspenakMeeusBoundaries[boundary];
  double left = espenak_meeus_formula(boundary, year);
  double right = espenak_meeus_formula(boundary + 1, year);
  return left + (smoothstep(center - half_width, center + half_width, year) *
                 (right - left));
}

/// @brief Least-squares trend of the last kTrendWindowDays of the IERS table.
/// @return Rate of change of Delta T in seconds per day.
constexpr auto table_trend() noexcept -> double {
  const auto& last = IERS_TABLE.back();
  double n = 0.0;
  double sum_t = 0.0;
  double sum_x = 0.0;
  double sum_tt = 0.0;
  double sum_tx = 0.0;
  for (auto ix = IERS_TABLE.size(); ix > 0; --ix) {
    const auto& item = IERS_TABLE[ix - 1];
    double t = item.mjd - last.mjd;
    if (t < -kTrendWindowDays) {
      break;
    }
    n += 1.0;
    sum_t += t;
    sum_x += item.delta_t;
    sum_tt += t * t;
    sum_tx += t * item.delta_t;
  }
  return ((n * sum_tx) - (sum_t * sum_x)) / ((n * sum_tt) - (sum_t * sum_t));
}

/// @brief Trend of Delta T at the end of the IERS table (seconds per day).
constexpr double kTableTrend = table_trend();

/// @brief Difference between the first entry of the IERS table and the
/// Espenak & Meeus (2006) formulas at its date (seconds).
constexpr double kTableStartOffset =
    IERS_TABLE.front().delta_t -
    espenak_meeus(mjd_to_year(IERS_TABLE.front().mjd));

// The alignment on the table must not overlap the last handover, and the
// table must end before the blend into the long-term parabola starts.
static_assert(kEspenakMeeusBoundaries.back() + (0.5 * kHandoverYears) <=
                  mjd_to_year(IERS_TABLE.front().mjd) - kHandoverYears,
              "The IERS table starts too early");
static_assert(mjd_to_year(IERS_TABLE.back().mjd) < kLongTermBlendStart,
              "The IERS table ends after the start of the long-term blend");

/// @brief Delta T before the IERS table.
///
/// Espenak & Meeus (2006) formulas, shifted progressively by
/// kTableStartOffset over the kHandoverYears preceding the table so as to
/// reach its first entry.
///
/// @param[in] year Decimal year.
/// @return Delta T in seconds.
constexpr auto before_table(double year) noexcept -> double {
  double start = mjd_to_year(IERS_TABLE.front().mjd);
  return espenak_meeus(year) +
         (smoothstep(start - kHandoverYears, start, year) * kTableStartOffset);
}

/// @brief Delta T after the IERS table.
///
/// Linear continuation of kTableTrend from the last entry of the table,
/// blended from kLongTermBlendStart to kLongTermBlendEnd with a smoothstep
/// weight into the Morrison & Stephenson (2004) parabola.
///
/// @param[in] mjd Modified Julian Date.
/// @return Delta T in seconds.
constexpr auto after_table(double mjd) noexcept -> double {
  const auto& last = IERS_TABLE.back();
  double trend = last.delta_t + (kTableTrend * (mjd - last.mjd));
  double year = mjd_to_year(mjd);
  return trend + (smoothstep(kLongTermBlendStart, kLongTermBlendEnd, year) *
                  (morrison_stephenson_parabola(year) - trend));
}

}  // namespace detail

/// @brief Convert Unix epoch to decimal year.
/// @param[in] epoch Seconds since 1970-01-01T00:00:00Z.
/// @return Decimal year.
constexpr auto epoch_to_year(double epoch) noexcept -> double {
  double jd = (epoch / numbers::kSecondsPerDay) + numbers::kUnixEpochJulianDay;
  double t = (jd - static_cast<double>(numbers::kJ2000JulianDay)) /
             static_cast<double>(numbers::kDaysPerCentury);
  return 2000.0 + (t * 100.0);
}

/// @brief Computes TT - UT1 (Delta T) using IERS tables for modern era
/// and historical polynomials for earlier periods.
/// @param[in] epoch Number of seconds elapsed since 1970-01-01T00:00:00Z.
/// @return Delta T in seconds.
///
/// @details
/// - Modern era (IERS table, from 1973): linear interpolation of the monthly
///   means of the IERS values and of the IERS Bulletin A predictions.
/// - After the table: continuation of the least-squares trend of the last two
///   years of the table, blended from 2050 to 2150 into the Morrison &
///   Stephenson (2004) parabola, which is used alone afterwards.
/// - Before the table: Espenak & Meeus (2006) polynomials, the parabola before
///   -500, joined by two-year handovers and aligned on the first entry of the
///   table over the two years preceding it.
///
/// The result is continuous at every epoch.
///
/// @note Values after the table are predictions. Over 1973-2026, extrapolating
/// the trend of the last two years missed the observed Delta T by 0.11 s RMS
/// (0.29 s at most) after one year, 1.0 s RMS (2.1 s) after five years and
/// 3.2 s RMS (6.8 s) after thirteen years.
/// @see https://www.iers.org for current Earth rotation data.
/// @see https://eclipse.gsfc.nasa.gov/SEhelp/deltatpoly2004.html
FES_MATH_CONSTEXPR auto fetch_delta_time(double epoch) -> double {
  double mjd = detail::epoch_to_mjd(epoch);

  // Before the table (the negated comparison also routes NaN, which then
  // propagates to the result)
  if (!(mjd >= IERS_TABLE.front().mjd)) {
    return detail::before_table(epoch_to_year(epoch));
  }

  // After the table
  if (mjd > IERS_TABLE.back().mjd) {
    return detail::after_table(mjd);
  }

  // Modern era: IERS table lookup with linear interpolation
  auto index = detail::lower_bound_by_mjd(IERS_TABLE, mjd);

  // Exact match on first entry
  if (index == 0) {
    return IERS_TABLE[0].delta_t;
  }

  // Interpolate between adjacent entries
  return detail::interpolate(IERS_TABLE[index - 1], IERS_TABLE[index], mjd);
}

}  // namespace fes
