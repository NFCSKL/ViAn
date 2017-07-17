#include "tag.h"

Tag::Tag()
{

}

/**
 * @brief Tag::add_frame
 * @param frame
 * Adds the newly tagged frame as a new poi
 * unless it's at the edge of a current poi or that frame already is tagged
 */
bool Tag::add_frame(int frame) {

}

void Tag::remove_frame(int frame) {
}
void Tag::add_interval(AnalysisInterval *an_interval){
    Analysis::add_interval(an_interval);
    merge_intervals();
}
/**
 * @brief Tag::merge_intervals
 * This function assumes m_intervals is sorted
 */
void Tag::merge_intervals(){
    std::set<AnalysisInterval*, poi_cmp> intervals = m_intervals;
    std::set<AnalysisInterval*, poi_cmp> res;
    auto it = intervals.begin();    
    AnalysisInterval* current = *it;
    it++;
    while (it != intervals.end()){
       if (current->getInterval().second >= (*it)->getInterval().first){ // you might want to change it to >=
           AnalysisInterval* merged = new AnalysisInterval(current->getInterval().first, (*it)->getInterval().second);
           merged = current;
           delete current;
       } else {
           res.insert(current);
           current = *(it);
       }
       it++;
    }
    res.insert(current);
    m_intervals = res;
}
