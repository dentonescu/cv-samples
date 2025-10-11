package dev.dmotservices.beans;

import java.io.Serializable;
import java.util.Objects;

public class EMail implements Serializable {

    public enum EMailType {
        OTHER("other"),
        PRIVATE("private"),
        WORK("work");

        private final String emailType;

        EMailType(String emailType) {
            this.emailType = emailType;
        }

        public String getEmailType() {
            return emailType;
        }
    }

    protected EMailType emailType;
    protected String address;

    public EMail(EMailType emailType, String address) {
        this.emailType = emailType;
        this.address = address;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public EMailType getEmailType() {
        return emailType;
    }

    public void setEmailType(EMailType emailType) {
        this.emailType = emailType;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EMail eMail = (EMail) o;
        return emailType == eMail.emailType && Objects.equals(address, eMail.address);
    }

    @Override
    public int hashCode() {
        return Objects.hash(emailType, address);
    }

    @Override
    public String toString() {
        return "EMail{" +
                "emailType=" + emailType +
                ", address='" + address + '\'' +
                '}';
    }
}
