#include <Testing/Testing.h>

#include <Stuff/Bounding_Volume_Hierarchy.h>

LST::Border construct_border(const glm::vec3& _position)
{
    constexpr glm::vec3 Size = {1, 1, 1};
    constexpr glm::vec3 Half_Size = Size * 0.5f;

    LST::Border_Construction_Helper helper;
    helper.consider_point(_position - Half_Size);
    helper.consider_point(_position + Half_Size);

    return LST::Border(helper);
}


INIT_TEST(BVH_Test_0_1)
{
    LST::Bounding_Volume_Hierarchy bvh;

    LDS::Vector<LST::Border> plug;

    const LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec& pairs = bvh.process(plug);
    EXPECT_EQUAL(pairs.size(), 0);

    bvh.process(1);
    EXPECT_EQUAL(pairs.size(), 0);
}

INIT_TEST(BVH_Test_No_Intersections)
{
    LDS::Vector<LST::Border> borders;
    for(unsigned int i = 0; i < 10; ++i)
        borders.push( construct_border( { 2.0f * (float)i, 0, 0 } ) );

    LST::Bounding_Volume_Hierarchy bvh;
    const LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec& pairs = bvh.process(borders);
    EXPECT_EQUAL(pairs.size(), 0);
}

INIT_TEST(BVH_Test_9_Intersections)
{
    LDS::Vector<LST::Border> borders;
    for(unsigned int i = 0; i < 10; ++i)
        borders.push( construct_border( { 0.75f * (float)i, 0, 0 } ) );

    LST::Bounding_Volume_Hierarchy bvh;
    const LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec& pairs = bvh.process(borders);
    EXPECT_EQUAL(pairs.size(), 9);
}

INIT_TEST(BVH_Test_Mutual_Intersections)
{
    LDS::Vector<LST::Border> borders;
    borders.push( construct_border({ 0, 0, 0 }) );
    borders.push( construct_border({ 0.3, 0, 0 }) );
    borders.push( construct_border({ 0.15, 0.15, 0 }) );

    LST::Bounding_Volume_Hierarchy bvh;
    const LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec& pairs = bvh.process(borders);
    EXPECT_EQUAL(pairs.size(), 3);
}

INIT_TEST(BVH_Test_Intersections_Spaced_Out)
{
    LDS::Vector<LST::Border> borders;
    /* 0 */ borders.push( construct_border({ 0, 0, 0 }) );
    /* 1 */ borders.push( construct_border({ 0.3, 0, 0 }) );
    /* 2 */ borders.push( construct_border({ 0.15, 0.15, 0 }) );

    /* 3 */ borders.push( construct_border({ 0, 10, 0 }) );
    /* 4 */ borders.push( construct_border({ 0, 10, 0.1 }) );

    /* 5 */ borders.push( construct_border({ 20, 0, 0 }) );
    /* 6 */ borders.push( construct_border({ 20, 1.1, 0 }) );
    /* 7 */ borders.push( construct_border({ 20, 0.9, 0 }) );

    LST::Bounding_Volume_Hierarchy bvh;
    const LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec& pairs = bvh.process(borders);
    EXPECT_EQUAL(pairs.size(), 6);

    LST::Bounding_Volume_Hierarchy::ID_Pair pair = pairs[0];
    EXPECT_EQUAL(pair.id_0, 0);
    EXPECT_EQUAL(pair.id_1, 1);

    pair = pairs[1];
    EXPECT_EQUAL(pair.id_0, 0);
    EXPECT_EQUAL(pair.id_1, 2);

    pair = pairs[2];
    EXPECT_EQUAL(pair.id_0, 1);
    EXPECT_EQUAL(pair.id_1, 2);

    pair = pairs[3];
    EXPECT_EQUAL(pair.id_0, 3);
    EXPECT_EQUAL(pair.id_1, 4);

    pair = pairs[4];
    EXPECT_EQUAL(pair.id_0, 5);
    EXPECT_EQUAL(pair.id_1, 7);

    pair = pairs[5];
    EXPECT_EQUAL(pair.id_0, 6);
    EXPECT_EQUAL(pair.id_1, 7);
}
