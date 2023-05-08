//
// Created by adam on 07/05/23.
//

#ifndef CMAKE_BUILD_DEBUG_ICP_PACMAN_LEVELENDSCENE_H
#define CMAKE_BUILD_DEBUG_ICP_PACMAN_LEVELENDSCENE_H

#include <string>
#include <regex>
#include <vector>
#include <QPoint>
#include "mapvector.h"


class LevelEndRepr {
public:
    LevelEndRepr();
    ~LevelEndRepr();

    std::vector<std::pair<QPoint, std::string>> dynamic_items;
private:
};

#endif //CMAKE_BUILD_DEBUG_ICP_PACMAN_LEVELENDSCENE_H
