#include <Testing/Testing.h>

#include <Stuff/Math_Stuff.h>
#include <Stuff/Border.h>


INIT_TEST(Border_Initialization_Test)
{
    glm::vec3 offset = {1, 1, 1};
    glm::vec3 limit = {5, 5, 5};

    LST::Border border;
    LST::Border_Construction_Helper helper;
    helper.consider_point(offset);
    helper.consider_point(limit);
    border.init(helper);

    glm::vec3 expected_center = {3, 3, 3};
    glm::vec3 expected_extents = {2, 2, 2};

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.center(), expected_center), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.extents(), expected_extents), true);

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_offset(), offset), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_limit(), limit), true);
}

INIT_TEST(Border_Points_Initialization_Test)
{
    LDS::Vector<glm::vec3> points;
    points.push({1, 5, 3});
    points.push({5, 1, 3});
    points.push({1, 3, 5});
    points.push({1, 1, 2});
    points.push({4, 3, 1});

    LST::Border border(points);

    glm::vec3 expected_center = {3, 3, 3};
    glm::vec3 expected_extents = {2, 2, 2};

    glm::vec3 expected_offset = {1, 1, 1};
    glm::vec3 expected_limit = {5, 5, 5};

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.center(), expected_center), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.extents(), expected_extents), true);

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_offset(), expected_offset), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_limit(), expected_limit), true);
}

INIT_TEST(Border_Raw_Data_Initialization_Test)
{
    LDS::Vector<float> raw_data;
    raw_data.push(1); raw_data.push(5); raw_data.push(3);
    raw_data.push(5); raw_data.push(1); raw_data.push(3);
    raw_data.push(1); raw_data.push(3); raw_data.push(5);
    raw_data.push(1); raw_data.push(1); raw_data.push(2);
    raw_data.push(4); raw_data.push(3); raw_data.push(1);

    LST::Border border(raw_data);

    glm::vec3 expected_center = {3, 3, 3};
    glm::vec3 expected_extents = {2, 2, 2};

    glm::vec3 expected_offset = {1, 1, 1};
    glm::vec3 expected_limit = {5, 5, 5};

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.center(), expected_center), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.extents(), expected_extents), true);

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_offset(), expected_offset), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border.calculate_limit(), expected_limit), true);
}


INIT_TEST(Border_Point_Containment_Test)
{
    glm::vec3 offset = {1, 1, 1};
    glm::vec3 limit = {5, 5, 5};

    LST::Border border;
    LST::Border_Construction_Helper helper;
    helper.consider_point(offset);
    helper.consider_point(limit);
    border.init(helper);


    glm::vec3 point = {2, 2, 2};
    EXPECT_EQUAL(border.point_is_inside(point), true);
    point = {1.1f, 2, 2};
    EXPECT_EQUAL(border.point_is_inside(point), true);
    point = {4.99f, 4.99f, 4.99f};
    EXPECT_EQUAL(border.point_is_inside(point), true);

    point = {0, 2, 2};
    EXPECT_EQUAL(border.point_is_inside(point), false);
    point = {2, 0, 2};
    EXPECT_EQUAL(border.point_is_inside(point), false);
    point = {2, 2, 0};
    EXPECT_EQUAL(border.point_is_inside(point), false);

    point = {1, 1, 1};
    EXPECT_EQUAL(border.point_is_inside(point), true);
    point = {5, 5, 5};
    EXPECT_EQUAL(border.point_is_inside(point), true);
}

INIT_TEST(Border_Intersection_Test)
{
    LST::Border_Construction_Helper helper;
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    LST::Border border_0;
    border_0.init(helper);

    helper.reset();
    helper.consider_point({3, 3, 3});
    helper.consider_point({7, 7, 7});
    LST::Border border_1;
    border_1.init(helper);

    EXPECT_EQUAL(border_0.intersects_with(border_1), true);
    EXPECT_EQUAL(border_1.intersects_with(border_0), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({4.999f, 4.999f, 4.999f});
    helper.consider_point({6, 6, 6});
    border_1.init(helper);

    EXPECT_EQUAL(border_0.intersects_with(border_1), true);
    EXPECT_EQUAL(border_1.intersects_with(border_0), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({5.0001f, 5.0001f, 5.0001f});
    helper.consider_point({6, 6, 6});
    border_1.init(helper);

    EXPECT_EQUAL(border_0.intersects_with(border_1), false);
    EXPECT_EQUAL(border_1.intersects_with(border_0), false);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-4.999f, -4.999f, -4.999f});
    helper.consider_point({-6, -6, -6});
    border_1.init(helper);

    EXPECT_EQUAL(border_0.intersects_with(border_1), false);
    EXPECT_EQUAL(border_1.intersects_with(border_0), false);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-4.999f, -4.999f, -4.999f});
    helper.consider_point({1.001f, 1.001f, 1.001f});
    border_1.init(helper);

    EXPECT_EQUAL(border_0.intersects_with(border_1), true);
    EXPECT_EQUAL(border_1.intersects_with(border_0), true);
}


INIT_TEST(Border_Update_Test)
{
    LST::Border_Construction_Helper helper;
    helper.consider_point({-2, -2, -2});
    helper.consider_point({2, 2, 2});

    LST::Border border_0(helper);
    LST::Border border_1(helper);

    LST::Border test_border_0 = border_0;
    LST::Border test_border_1 = border_1;

    glm::mat4x4 translation_matrix =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    glm::mat4x4 rotation_matrix;
    glm::mat4x4 transformation_matrix;

    translation_matrix[3][0] = 0;
    rotation_matrix = translation_matrix;
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_0.update(transformation_matrix);

    translation_matrix[3][0] = 5;
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_1.update(transformation_matrix);

    EXPECT_EQUAL(test_border_0.intersects_with(test_border_1), false);
    EXPECT_EQUAL(test_border_1.intersects_with(test_border_0), false);


    translation_matrix[3][0] = 0;
    rotation_matrix = LST::Math::calculate_rotation_matrix({0, 0, LST::Math::PI});
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_0.update(transformation_matrix);

    translation_matrix[3][0] = 5;
    rotation_matrix = LST::Math::calculate_rotation_matrix({0, 0, LST::Math::PI});
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_1.update(transformation_matrix);

    EXPECT_EQUAL(test_border_0.intersects_with(test_border_1), false);
    EXPECT_EQUAL(test_border_1.intersects_with(test_border_0), false);


    translation_matrix[3][0] = 0;
    rotation_matrix = LST::Math::calculate_rotation_matrix({0, 0, LST::Math::QUARTER_PI});
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_0.update(transformation_matrix);

    translation_matrix[3][0] = 5;
    rotation_matrix = LST::Math::calculate_rotation_matrix({0, 0, LST::Math::QUARTER_PI});
    transformation_matrix = translation_matrix * rotation_matrix;
    test_border_1.update(transformation_matrix);

    EXPECT_EQUAL(test_border_0.intersects_with(test_border_1), true);
    EXPECT_EQUAL(test_border_1.intersects_with(test_border_0), true);
}


INIT_TEST(Border_Expansion_Test)
{
    LST::Border_Construction_Helper helper;
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    LST::Border border_0;
    border_0.init(helper);

    helper.reset();
    helper.consider_point({3, 3, 3});
    helper.consider_point({7, 7, 7});
    LST::Border border_1;
    border_1.init(helper);

    border_0.expand_with(border_1);

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border_0.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border_0.calculate_limit(), {7, 7, 7}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-5, -5, -5});
    helper.consider_point({-1, -1, -1});
    border_1.init(helper);

    border_0.expand_with(border_1);

    EXPECT_EQUAL(LST::Math::vecs_are_equal(border_0.calculate_offset(), {-5, -5, -5}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(border_0.calculate_limit(), {5, 5, 5}), true);
}

INIT_TEST(Border_Exclusion_Test)
{
    LST::Border_Construction_Helper helper;
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    LST::Border border_0;
    border_0.init(helper);

    helper.reset();
    helper.consider_point({3, 3, 3});
    helper.consider_point({7, 7, 7});
    LST::Border border_1;
    border_1.init(helper);

    LST::Border test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {3, 3, 3}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {5, 5, 5}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-5, -5, -5});
    helper.consider_point({-1, -1, -1});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), false);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({6, 2, 6});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 2.01, 1});
    helper.consider_point({6, 3, 1});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), false);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({6, 2, 6});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 1.9, 1});
    helper.consider_point({6, 3, 6});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1.9, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {6, 2, 6}), true);


    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-5, -5, -5});
    helper.consider_point({0.001f, 0.001f, 0.001f});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {0, 0, 0}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {0.001f, 0.001f, 0.001f}), true);


    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({4, 4, 4});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {4, 4, 4}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({4, 4, 4});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_1.init(helper);

    test_border = border_0 && border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {4, 4, 4}), true);
}

INIT_TEST(Border_Inclusion_Test)
{
    LST::Border_Construction_Helper helper;
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    LST::Border border_0;
    border_0.init(helper);

    helper.reset();
    helper.consider_point({3, 3, 3});
    helper.consider_point({7, 7, 7});
    LST::Border border_1;
    border_1.init(helper);

    LST::Border test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {7, 7, 7}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-5, -5, -5});
    helper.consider_point({-1, -1, -1});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {-5, -5, -5}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {5, 5, 5}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({6, 2, 6});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 2.01, 1});
    helper.consider_point({6, 3, 1});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {6, 3, 6}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({6, 2, 6});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 1.9, 1});
    helper.consider_point({6, 3, 6});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {1, 1, 1}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {6, 3, 6}), true);


    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({-5, -5, -5});
    helper.consider_point({0.001f, 0.001f, 0.001f});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {-5, -5, -5}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {5, 5, 5}), true);


    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({4, 4, 4});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {0, 0, 0}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {5, 5, 5}), true);


    helper.reset();
    helper.consider_point({1, 1, 1});
    helper.consider_point({4, 4, 4});
    border_0.init(helper);

    helper.reset();
    helper.consider_point({0, 0, 0});
    helper.consider_point({5, 5, 5});
    border_1.init(helper);

    test_border = border_0 || border_1;

    EXPECT_EQUAL(test_border.valid(), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_offset(), {0, 0, 0}), true);
    EXPECT_EQUAL(LST::Math::vecs_are_equal(test_border.calculate_limit(), {5, 5, 5}), true);
}
