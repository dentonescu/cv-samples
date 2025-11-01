package dev.dmotservices.beans;

import java.net.URL;
import java.util.Objects;

public class User {

    protected Long id;
    protected String firstName;
    protected String lastName;
    protected String userName;
    protected URL urlProfilePicture;
    protected String primaryEmail;
    protected String primaryTelephone;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public URL getUrlProfilePicture() {
        return urlProfilePicture;
    }

    public void setUrlProfilePicture(URL urlProfilePicture) {
        this.urlProfilePicture = urlProfilePicture;
    }

    public String getPrimaryEmail() {
        return primaryEmail;
    }

    public void setPrimaryEmail(String primaryEmail) {
        this.primaryEmail = primaryEmail;
    }

    public String getPrimaryTelephone() {
        return primaryTelephone;
    }

    public void setPrimaryTelephone(String primaryTelephone) {
        this.primaryTelephone = primaryTelephone;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        User user = (User) o;
        return Objects.equals(id, user.id) && Objects.equals(firstName, user.firstName) && Objects.equals(lastName, user.lastName) && Objects.equals(userName, user.userName) && Objects.equals(urlProfilePicture, user.urlProfilePicture) && Objects.equals(primaryEmail, user.primaryEmail) && Objects.equals(primaryTelephone, user.primaryTelephone);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, firstName, lastName, userName, urlProfilePicture, primaryEmail, primaryTelephone);
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", userName='" + userName + '\'' +
                ", urlProfilePicture=" + urlProfilePicture +
                ", primaryEmail='" + primaryEmail + '\'' +
                ", primaryTelephone='" + primaryTelephone + '\'' +
                '}';
    }
}
