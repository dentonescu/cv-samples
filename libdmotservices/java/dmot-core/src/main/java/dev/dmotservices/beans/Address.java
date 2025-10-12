package dev.dmotservices.beans;

import java.io.Serializable;
import java.util.Objects;

public class Address implements Serializable {

    public enum AddressType {
        OTHER("other"),
        PRIVATE("private"),
        WORK("work");

        private final String addressType;

        AddressType(String addressType) {
            this.addressType = addressType;
        }

        public String getAddressType() {
            return addressType;
        }
    }

    protected AddressType addressType;
    protected String street;
    protected String postalCode;
    protected String city;
    protected String region;
    protected String country;

    public AddressType getAddressType() {
        return addressType;
    }

    public void setAddressType(AddressType addressType) {
        this.addressType = addressType;
    }

    public String getStreet() {
        return street;
    }

    public void setStreet(String street) {
        this.street = street;
    }

    public String getPostalCode() {
        return postalCode;
    }

    public void setPostalCode(String postalCode) {
        this.postalCode = postalCode;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getRegion() {
        return region;
    }

    public void setRegion(String region) {
        this.region = region;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Address address = (Address) o;
        return addressType == address.addressType && Objects.equals(street, address.street) && Objects.equals(postalCode, address.postalCode) && Objects.equals(city, address.city) && Objects.equals(region, address.region) && Objects.equals(country, address.country);
    }

    @Override
    public int hashCode() {
        return Objects.hash(addressType, street, postalCode, city, region, country);
    }

    @Override
    public String toString() {
        return "Address{" +
                "addressType=" + addressType +
                ", street='" + street + '\'' +
                ", postalCode='" + postalCode + '\'' +
                ", city='" + city + '\'' +
                ", region='" + region + '\'' +
                ", country='" + country + '\'' +
                '}';
    }
}