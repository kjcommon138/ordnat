/* Standard ordnat library. DO NOT EDIT. */
#ifndef __ORDNAT_H__
#define __ORDNAT_H__

/* Range macros */
#define foreach_joint(joint_name, item_name, joint_item, offset_idx, loop) \
for (__typeof__ ((joint_name).joint_item) item_name = ((joint_name).joint_item); \
    item_name < (__typeof__ ((joint_name).joint_item)) (((uintptr_t)(joint_name).memory_block) + ((joint_name).offset[offset_idx])); \
    ++item_name) loop

#define foreach_range_joint(joint_name, item_name, joint_item, itr, offset_idx, loop) \
{ size_t itr = 0; for (__typeof__ ((joint_name).joint_item) item_name = ((joint_name).joint_item); \
    item_name < (__typeof__ ((joint_name).joint_item)) (((uintptr_t)(joint_name).memory_block) + ((joint_name).offset[offset_idx])); \
    ++item_name, ++itr) loop }

#define for_range_joint(joint_name, item_name, joint_item, start, end, step, itr, offset_idx, loop) \
{ __typeof__ (start) itr = (start); for (__typeof__ ((joint_name).joint_item) item_name = ((joint_name).joint_item) + (start); \
    itr < min( \
      ((joint_name).joint_item) + (end), \
      (__typeof__ ((joint_name).joint_item)) (((uintptr_t)(joint_name).memory_block) + ((joint_name).offset[offset_idx])) \
      ); \
    item_name += (step), itr += (step)) loop }

#define for_range_down_joint(joint_name, item_name, joint_item, start, end, step, itr, loop) \
{ __typeof__ (start) itr = (start); for (__typeof__ ((joint_name).joint_item) item_name = ((joint_name).joint_item) + (start); \
    itr >= max( \
      ((joint_name).joint_item) + (end), \
      ((joint_name).joint_item) \
      ); \
    item_name -= (step), itr -= (step)) loop }

/* Source: https://stackoverflow.com/a/3437484 */
#define max(a,b)           \
({                         \
  __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a > _b ? _a : _b;       \
})

#define min(a,b)           \
({                         \
  __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a < _b ? _a : _b;       \
})

#endif /* __ORDNAT_H__ */
