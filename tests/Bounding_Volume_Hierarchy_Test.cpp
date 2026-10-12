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
    LDS::Vector<LST::Border> plug;

    LST::BVH::Context context;

    LST::Bounding_Volume_Hierarchy bvh;

    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 0);

    context.registered_borders.add(plug);

    bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 0);
}

INIT_TEST(BVH_Test_No_Intersections)
{
    LDS::Vector<LST::Border> borders;
    for(unsigned int i = 0; i < 10; ++i)
        borders.push( construct_border( { 2.0f * (float)i, 0, 0 } ) );

    LST::BVH::Context context;
    context.registered_borders.add(borders);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 0);
}

INIT_TEST(BVH_Test_9_Intersections)
{
    LDS::Vector<LST::Border> borders;
    for(unsigned int i = 0; i < 10; ++i)
        borders.push( construct_border( { 0.75f * (float)i, 0, 0 } ) );

    LST::BVH::Context context;
    context.registered_borders.add(borders);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 9);
}

INIT_TEST(BVH_Test_Mutual_Intersections)
{
    LDS::Vector<LST::Border> borders;
    borders.push( construct_border({ 0, 0, 0 }) );
    borders.push( construct_border({ 0.3, 0, 0 }) );
    borders.push( construct_border({ 0.15, 0.15, 0 }) );

    LST::BVH::Context context;
    context.registered_borders.add(borders);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
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

    LST::BVH::Context context;
    context.registered_borders.add(borders);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 6);

    L_ASSERT(pairs.size() == 6);

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


INIT_TEST(BVH_Test_2_Groups_Intersections)
{
    LDS::Vector<LST::Border> borders_0;
    /* 0 */ borders_0.push( construct_border({ 0, 0, 0 }) );
    /* 1 */ borders_0.push( construct_border({ 0.3, 0, 0 }) );

    /* 2 */ borders_0.push( construct_border({ 0, 10, 0 }) );

    /* 3 */ borders_0.push( construct_border({ 20, 0, 0 }) );
    /* 4 */ borders_0.push( construct_border({ 20, 0.9, 0 }) );

    LDS::Vector<LST::Border> borders_1;
    /* 0 */ borders_1.push( construct_border({ 0.15, 0.15, 0 }) );
    /* 1 */ borders_1.push( construct_border({ 0, 10, 0.1 }) );
    /* 2 */ borders_1.push( construct_border({ 20, 1.1, 0 }) );

    LST::BVH::Context context;
    context.registered_borders.add(borders_0);
    context.registered_borders.add(borders_1);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 4);

    const LST::BVH::Registered_Borders& registered_borders = context.registered_borders;

    LST::BVH::ID_Pair pair = pairs[0];
    LST::BVH::Registered_Borders::Border_Location location_0 = registered_borders.calculate_border_location(pair.id_0);
    LST::BVH::Registered_Borders::Border_Location location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 0 );
    EXPECT_EQUAL( location_1.group, 1 );
    EXPECT_EQUAL( location_0.index, 0 );
    EXPECT_EQUAL( location_1.index, 0 );

    pair = pairs[1];
    location_0 = registered_borders.calculate_border_location(pair.id_0);
    location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 0 );
    EXPECT_EQUAL( location_1.group, 1 );
    EXPECT_EQUAL( location_0.index, 1 );
    EXPECT_EQUAL( location_1.index, 0 );

    pair = pairs[2];
    location_0 = registered_borders.calculate_border_location(pair.id_0);
    location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 0 );
    EXPECT_EQUAL( location_1.group, 1 );
    EXPECT_EQUAL( location_0.index, 2 );
    EXPECT_EQUAL( location_1.index, 1 );

    pair = pairs[3];
    location_0 = registered_borders.calculate_border_location(pair.id_0);
    location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 0 );
    EXPECT_EQUAL( location_1.group, 1 );
    EXPECT_EQUAL( location_0.index, 4 );
    EXPECT_EQUAL( location_1.index, 2 );
}


INIT_TEST(BVH_Test_3_Groups_Intersections)
{
    LDS::Vector<LST::Border> borders_0;
    /* 0 */ borders_0.push( construct_border({ 0, 0, 0 }) );
    /* 1 */ borders_0.push( construct_border({ -0.6, 0.5, 0 }) );

    LDS::Vector<LST::Border> borders_1;
    /* 0 */ borders_1.push( construct_border({ 0.5, 0.15, 0 }) );

    LDS::Vector<LST::Border> borders_2;
    /* 0 */ borders_2.push( construct_border({ 1.2, 0, 0 }) );
    /* 2 */ borders_2.push( construct_border({ 1.7, 0.4, 0 }) );

    LST::BVH::Context context;
    context.registered_borders.add(borders_0);
    context.registered_borders.add(borders_1);
    context.registered_borders.add(borders_2);

    LST::Bounding_Volume_Hierarchy bvh;
    LST::Bounding_Volume_Hierarchy::ID_Pairs_Vec pairs = bvh.process(context);
    EXPECT_EQUAL(pairs.size(), 2);

    const LST::BVH::Registered_Borders& registered_borders = context.registered_borders;

    LST::BVH::ID_Pair pair = pairs[0];
    LST::BVH::Registered_Borders::Border_Location location_0 = registered_borders.calculate_border_location(pair.id_0);
    LST::BVH::Registered_Borders::Border_Location location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 0 );
    EXPECT_EQUAL( location_1.group, 1 );
    EXPECT_EQUAL( location_0.index, 0 );
    EXPECT_EQUAL( location_1.index, 0 );

    pair = pairs[1];
    location_0 = registered_borders.calculate_border_location(pair.id_0);
    location_1 = registered_borders.calculate_border_location(pair.id_1);
    EXPECT_EQUAL( location_0.group, 1 );
    EXPECT_EQUAL( location_1.group, 2 );
    EXPECT_EQUAL( location_0.index, 0 );
    EXPECT_EQUAL( location_1.index, 0 );
}
