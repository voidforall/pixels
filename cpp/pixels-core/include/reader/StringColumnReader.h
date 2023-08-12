//
// Created by liyu on 3/20/23.
//

#ifndef PIXELS_STRINGCOLUMNREADER_H
#define PIXELS_STRINGCOLUMNREADER_H

#include "reader/ColumnReader.h"
#include "encoding/RunLenIntDecoder.h"

class StringColumnReader: public ColumnReader {
public:
    explicit StringColumnReader(std::shared_ptr<TypeDescription> type);
	~StringColumnReader();
    void close() override;
    void read(std::shared_ptr<ByteBuffer> input,
              pixels::proto::ColumnEncoding & encoding,
              int offset, int size, int pixelStride,
              int vectorIndex, std::shared_ptr<ColumnVector> vector,
              pixels::proto::ColumnChunkIndex & chunkIndex,
			  std::shared_ptr<PixelsBitMask> filterMask) override;

private:
    /**
     * RLE decoder of string content element length if no dictionary encoded.
     */
    std::shared_ptr<ByteBuffer> contentBuf;
    // The string content in dictionary.
	std::shared_ptr<ByteBuffer> dictContentBuf;

    std::shared_ptr<ByteBuffer> startsBuf;
    /**
     * The start offset of the current element in the content if not dictionary encoded.
     */
    int currentStart;
    /**
     * The next element in the content if not dictionary encoded.
     */
    int nextStart;

	std::shared_ptr<RunLenIntDecoder> contentDecoder;
    int bufferOffset;
    int dictContentOffset;
    int dictStartsOffset;

	int * dictStarts;
    int startsLength;
    /**
     * In this method, we have reduced most of significant memory copies.
     */
    void readContent(std::shared_ptr<ByteBuffer> input,
                      uint32_t inputLength, pixels::proto::ColumnEncoding & encoding);
};
#endif //PIXELS_STRINGCOLUMNREADER_H
