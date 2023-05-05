//
// Created by adam on 03/05/23.
//

#include <QList>

#include "../Headers/pathfinder.h"


QList<char> PathFinder::find_path(QPoint start, QPoint end, const MapVector& map) {
    QList<char> final_path;
    QList<PFListItem> open;
    QList<PFListItem> closed;

    open.append({start, 'N', PathFinder::heuristic(start, end), 0});

    while (true) {
        if (open.empty()){
            throw PathFinder::NoPathException();
        }

        PFListItem *lowest_cost_item = std::min_element(
                open.begin(),
                open.end(),
                [](PFListItem a, PFListItem b){return a.cost < b.cost;}
        );
        if (lowest_cost_item == open.end()) throw PathFinder::NoPathException();

        if (lowest_cost_item->point == end){
            PFListItem *current_item = lowest_cost_item;
            while (current_item->point != start){
                final_path.push_front(current_item->direction);
                QPoint prev_point;
                switch (current_item->direction) {
                    case 'U':
                        prev_point = {current_item->point.x(), current_item->point.y() + 1};
                        break;
                    case 'D':
                        prev_point = {current_item->point.x(), current_item->point.y() - 1};
                        break;
                    case 'R':
                        prev_point = {current_item->point.x() - 1, current_item->point.y()};
                        break;
                    case 'L':
                        prev_point = {current_item->point.x() + 1, current_item->point.y()};
                        break;
                    default:
                        throw PathFinder::NoPathException();
                }
                current_item = std::find_if(closed.begin(), closed.end(), [&prev_point](PFListItem item){return item.point == prev_point;});
                if (current_item == closed.end()) throw PathFinder::NoPathException();
            }

            return final_path;
        }

        qreal h = 0, d = 0;
        int x, y;
        QPoint current_point = {0, 0};
        MapVector::MapObjectType current_cell;
        bool is_in_closed = false;

        x = lowest_cost_item->point.x();
        y = lowest_cost_item->point.y();

        current_point = {x + 1, y};
        is_in_closed = std::any_of(closed.begin(), closed.end(), [&current_point](PFListItem item){return item.point == current_point;});
        if (!is_in_closed) {
            current_cell = map.get_vector()[y][x + 1];
            if (current_cell != MapVector::Wall){
                h = PathFinder::heuristic(QPoint(x, y), end);
                d = lowest_cost_item->distance + 1;
                open.append({QPoint(x + 1, y), 'R', h + d, d});
            }
        }


        current_point = {x - 1, y};
        is_in_closed = std::any_of(closed.begin(), closed.end(), [&current_point](PFListItem item){return item.point == current_point;});
        if (!is_in_closed) {
            current_cell = map.get_vector()[y][x - 1];
            if (current_cell != MapVector::Wall){
                h = PathFinder::heuristic(QPoint(x, y), end);
                d = lowest_cost_item->distance + 1;
                open.append({QPoint(x - 1, y), 'L', h + d, d});
            }
        }

        current_point = {x, y + 1};
        is_in_closed = std::any_of(closed.begin(), closed.end(), [&current_point](PFListItem item){return item.point == current_point;});
        if (!is_in_closed) {
            current_cell = map.get_vector()[y + 1][x];
            if (current_cell != MapVector::Wall){
                h = PathFinder::heuristic(QPoint(x, y), end);
                d = lowest_cost_item->distance + 1;
                open.append({QPoint(x, y + 1), 'D', h + d, d});
            }
        }

        current_point = {x, y - 1};
        is_in_closed = std::any_of(closed.begin(), closed.end(), [&current_point](PFListItem item){return item.point == current_point;});
        if (!is_in_closed) {
            current_cell = map.get_vector()[y - 1][x];
            if (current_cell != MapVector::Wall){
                h = PathFinder::heuristic(QPoint(x, y), end);
                d = lowest_cost_item->distance + 1;
                open.append({QPoint(x, y - 1), 'U', h + d, d});
            }
        }

        QList<PFListItem *> to_delete;
        for (int i = 0; i < open.size(); ++i) {
            for (int j = i + 1; j < open.size(); ++j) {
                if (open[j].point == open[i].point){
                    if (open[j].cost < open[i].cost) {
                        to_delete.append(&open[i]);
                        break;
                    } else {
                        to_delete.append(&open[j]);
                    }
                }
            }
        }

        for (auto *item : to_delete) {
            open.removeOne(*item);
        }

        closed.append(*lowest_cost_item);
        open.removeOne(*lowest_cost_item);
    }
}

char PathFinder::get_next_direction(QPoint start, QPoint end, const MapVector& map) {
    return this->find_path(start, end, map)[0];
}

qreal PathFinder::heuristic(QPoint start, QPoint end) {
    return sqrt(abs(end.x() - start.x()) + abs(end.y() - start.y()));
}
