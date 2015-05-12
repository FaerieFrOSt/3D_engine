
#ifndef _UTILS_H_
# define _UTILS_H_

#define min(a, b)				((a) < (b) ? (a) : (b))
#define max(a, b)				((a) > (b) ? (a) : (b))
#define clamp(a, _min, _max)	max(min(a, _max), _min)

#endif /* !_UTILS_H_ */
