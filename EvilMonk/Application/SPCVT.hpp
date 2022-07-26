#ifndef SPCVT_H
#define SPCVT_H

#include <memory>
#include <functional>
#include <list>
#include <vector>

class SPCVT {
public:

    SPCVT(int radius, bool dense = false);

    void FOV(int origin_x, int origin_y,
        std::function<bool(int, int)> DoesBlockVision,
        std::function<void(int, int)> SetVisible) const;

    bool LOS(int a_x, int a_y, int b_x, int b_y,
        std::function<bool(int, int)> DoesBlockVision,
        std::function<void(int, int)> TraceOut = nullptr) const;

private:
    struct Trie {
        int x;
        int y;
        int los_key;
        Trie* antecedent;
        std::unique_ptr<Trie> descendants[9];

        Trie(int x, int y, int los_key, Trie* antecedent);

        static void AddPath(Trie* root,
            std::vector<std::list<Trie*> >& fast_los_map,
            int radius, int tx, int ty);
        void PreOrder(std::function<bool(int, int)> ShouldStop) const;
    };

    std::unique_ptr<Trie> root;
    std::vector<std::list<Trie*> > fast_los_map;
    int radius;
};

#endif
