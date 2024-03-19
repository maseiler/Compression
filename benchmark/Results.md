# Results

After I set up my benchmarks and executed them, I had to find out that my LZ77 implementation is fault :'(.
Unfortunately, I was not able to fix the bug in the given time... However, I'd like to explain anticipated results
below.

Run-length encoding is very effective for consecutive, recurring symbols. It finds wide adoption in media compression,
for example for images. My goal was to create a benchmark with an actual image, however, due to my byte-based
implementation, it won't be very effective.

Dictionary encoding is effective in finding respective patterns. For LZ77, the setting of the window size can have a
big impact on the compression ratio.

Ultimately, the compression ratio of the algorithms heavily depends on the given data. My data generator creates very
synthetic data, which hardly represents the real world. Sometimes, data has the form of tables (relational databases),
whose memory layout (i.e. row layout or column layout) can result in very different outcomes, although it represents 
the same data.

---

Things that I would have liked to tackle:
- fix LZ77 implementation
- benchmark a variety of data sets
  - [Pion samples](https://opendata.cern.ch/record/15012) from ATLAS experiment
  - image/photo
  - text/book
- benchmark with different parameters
- improve algorithms as mentioned in readme
