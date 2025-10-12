package dev.dmotservices.api;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.UUID;

public class MultiPartFormDataRequest extends Request {

    public static final String BOUNDARY = "Boundary" + UUID.randomUUID();

    public List<Segment> segments = new ArrayList<>();

    public static class Segment {
        protected MimeType mimeType;
        protected String fieldName;
        protected String fileName;
        protected Object data;

        /*
         * generated
         */

        public MimeType getMimeType() {
            return mimeType;
        }

        public void setMimeType(MimeType mimeType) {
            this.mimeType = mimeType;
        }

        public String getFieldName() {
            return fieldName;
        }

        public void setFieldName(String fieldName) {
            this.fieldName = fieldName;
        }

        public String getFileName() {
            return fileName;
        }

        public void setFileName(String fileName) {
            this.fileName = fileName;
        }

        public Object getData() {
            return data;
        }

        public void setData(Object data) {
            this.data = data;
        }

        @Override
        public boolean equals(Object o) {
            if (o == null || getClass() != o.getClass()) return false;
            Segment segment = (Segment) o;
            return mimeType == segment.mimeType && Objects.equals(fieldName, segment.fieldName) && Objects.equals(fileName, segment.fileName) && Objects.equals(data, segment.data);
        }

        @Override
        public int hashCode() {
            return Objects.hash(mimeType, fieldName, fileName, data);
        }

        @Override
        public String toString() {
            return "Segment{" +
                    "mimeType=" + mimeType +
                    ", fieldName='" + fieldName + '\'' +
                    ", fileName='" + fileName + '\'' +
                    ", data=" + data +
                    '}';
        }
    }

    public MultiPartFormDataRequest() {
        headers.put("Content-Type", MimeType.MULTIPART_FORM_DATA.getMimeType() + "; boundary=" + BOUNDARY);
        requestMethod = RequestMethod.POST.getRequestMethod();
        postBody = null;
    }

    public void attachData(Segment segment) {
        if (null == segment || null == segment.getFieldName() ||
                null == segment.getData())
            throw new IllegalArgumentException("A segment must have at least a field name and data.");
        segments.add(segment);
    }

    /*
     * generated
     */

    public List<Segment> getSegments() {
        return segments;
    }

    public void setSegments(List<Segment> segments) {
        this.segments = segments;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        MultiPartFormDataRequest that = (MultiPartFormDataRequest) o;
        return Objects.equals(segments, that.segments);
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(segments);
    }

    @Override
    public String toString() {
        return "MultiPartFormDataRequest{" +
                "segments=" + segments +
                ", baseUrl=" + baseUrl +
                ", endpointUrl=" + endpointUrl +
                ", headers=" + headers +
                ", params=" + params +
                ", postBody='" + postBody + '\'' +
                ", requestMethod='" + requestMethod + '\'' +
                '}';
    }
}
