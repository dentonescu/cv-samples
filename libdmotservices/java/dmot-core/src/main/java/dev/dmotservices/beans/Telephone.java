package dev.dmotservices.beans;

import java.io.Serializable;
import java.util.Objects;

public class Telephone implements Serializable {

    public enum TelephoneType {
        FAX_PRIVATE("fax_private"),
        FAX_WORK("fax_work"),
        LANDLINE_PRIVATE("landline_private"),
        LANDLINE_WORK("landline_work"),
        MOBILE_PRIVATE("mobile_private"),
        MOBILE_WORK("mobile_work"),
        OTHER("other"),
        OTHER_MOBILE("other_mobile"),
        OTHER_PRIVATE("other_private"),
        OTHER_WORK("other_work");

        private final String telephoneType;

        TelephoneType(String telephoneType) {
            this.telephoneType = telephoneType;
        }

        public String getTelephoneType() {
            return telephoneType;
        }
    }

    protected TelephoneType telephoneType;
    protected String number;

    public Telephone(TelephoneType telephoneType, String number) {
        this.telephoneType = telephoneType;
        this.number = number;
    }

    public TelephoneType getTelephoneType() {
        return telephoneType;
    }

    public void setTelephoneType(TelephoneType telephoneType) {
        this.telephoneType = telephoneType;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Telephone telephone = (Telephone) o;
        return telephoneType == telephone.telephoneType && Objects.equals(number, telephone.number);
    }

    @Override
    public int hashCode() {
        return Objects.hash(telephoneType, number);
    }

    @Override
    public String toString() {
        return "Telephone{" +
                "telephoneType=" + telephoneType +
                ", number='" + number + '\'' +
                '}';
    }
}
