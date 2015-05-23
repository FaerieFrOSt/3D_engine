
#ifndef _UTILS_H_
# define _UTILS_H_

#define min(a, b)				((a) < (b) ? (a) : (b))
#define max(a, b)				((a) > (b) ? (a) : (b))
#define clamp(a, _min, _max)	max(min(a, _max), _min)

#define swap(a, b)				do { __typeof__ (a) _tmp = (a); \
										(a) = (b); (b) = _tmp; } while (0)
#define abs(a)					((a) < 0 ? -(a) : (a))

#endif /* !_UTILS_H_ */
