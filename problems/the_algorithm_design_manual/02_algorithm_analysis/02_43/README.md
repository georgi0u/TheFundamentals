# 02-43

You are given a set `S` of `n` numbers. You must pick a subset `S'` of
`k` numbers from `S` such that the probability of each element of `S`
occurring in `S'` is equal (i.e., each is selected with probability
`k/n`). You may make only one pass over the numbers. What if `n` is unknown?

## Notes

I think the last part of this question is slightly misstated. If `n` were truly
unknown, it would be nearly impossible to iterate over `S` or pull indexes from it,
without going out of the bounds of the set. Instead, after researching
the problem a bit, I think the author is trying to allude to the technique of 
*Reservoir Sampling*, where `n` is either infinite or exceptionally large.
