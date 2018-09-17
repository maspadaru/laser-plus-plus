#include "formula/extended/atom.h"
#include "formula/extended/diamond.h"
#include "formula/grounding.h"
#include "rule/rule.h"
#include "util/timeline.h"
#include <gtest/gtest.h>

TEST(DiamondTest, DiamondAtom) {
    // ASSERT_EQ(-1.0, squareRoot(-15.0));
    // ASSERT_EQ(-1.0, squareRoot(-0.2));
    auto body_child_atom = new laser::formula::Atom("p", {"X"});
    auto body = new laser::formula::Diamond(body_child_atom);
    auto head = new laser::formula::Atom("a", {"X"});
    auto rule = laser::rule::Rule(head, body_child_atom);
    laser::util::Timeline timeline;
    for (int i = 1; i < 5; i++) {
        timeline.increment_time();
        std::unordered_map<std::string, std::vector<laser::formula::Grounding>>
            facts;
        facts.try_emplace("p");
        auto &fact_vector = facts["p"];
        ASSERT_EQ(0, fact_vector.size());
        auto grounding_bad = laser::formula::Grounding(i, i, i, 99);
        fact_vector.push_back(grounding_bad);
        auto grounding_good = laser::formula::Grounding(22, i, i, 99);
        fact_vector.push_back(grounding_good);
        rule.expire_outdated_groundings(timeline);
        rule.evaluate(timeline, facts);
        bool has_derived_conclusions = rule.derive_conclusions(timeline);

        auto &tstvct = facts["p"];
        ASSERT_EQ(2, tstvct.size());
        EXPECT_TRUE(has_derived_conclusions);
        EXPECT_TRUE(rule.get_head().is_satisfied());
    }

    for (int i = 5; i < 9; i++) {
        timeline.increment_time();
        std::unordered_map<std::string, std::vector<laser::formula::Grounding>>
            facts;
        facts.try_emplace("p");
        auto &fact_vector = facts["p"];
        auto grounding_bad = laser::formula::Grounding(i, i, i, 99);
        fact_vector.push_back(grounding_bad);
        rule.expire_outdated_groundings(timeline);
        rule.evaluate(timeline, facts);
        bool has_derived_conclusions = rule.derive_conclusions(timeline);

        EXPECT_FALSE(has_derived_conclusions);
        EXPECT_FALSE(rule.get_head().is_satisfied());
    }
}
