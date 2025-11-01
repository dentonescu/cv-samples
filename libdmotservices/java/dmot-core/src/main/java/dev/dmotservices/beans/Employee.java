package dev.dmotservices.beans;

import dev.dmotservices.datatypes.organization.OrganizationUnit;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

public class Employee {
    protected String status;
    protected Long employeeId;
    protected String employeePublicCode;
    protected String userName;
    protected String title;
    protected String firstName;
    protected String lastName;
    protected String gender;
    protected String civilStatus;
    protected String honorific;
    protected String birthName;
    protected String citizenship;
    protected String nationalityDenonym;
    protected String religion;
    protected String dateOfBirth;
    protected String entryDate;
    protected String exitDate;
    protected String disabilityGrade;
    protected String disabilityValidFrom;
    protected String disabilityValidTo;
    protected List<Long> supervisorIds = new ArrayList<>();
    protected List<Telephone> telephoneNumbers = new ArrayList<>();
    protected List<Address> addresses = new ArrayList<>();
    protected List<EMail> emailAddresses = new ArrayList<>();
    protected String settlementArea;
    protected OrganizationUnit organizationUnit;
    protected List<String> roles = new ArrayList<>();

    public void addRole(String role) {
        if (null == role || role.isBlank())
            return;
        if (!roles.contains(role))
            roles.add(role.trim());
    }

    public void addRoles(List<String> roles) {
        if (null == roles)
            return;
        for (String role : roles) {
            addRole(role);
        }
    }

    public void addRoles(String csvRoles) {
        if (null == csvRoles || csvRoles.isBlank())
            return;
        addRoles(Arrays.asList(csvRoles.split(",")));
    }

    public boolean hasRole(String role) {
        if (null == role || role.isBlank())
            throw new IllegalArgumentException("Rejecting an attempt to check for a blank or empty role.");
        return roles.contains(role.trim());
    }

    /*
     * generated
     */

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public Long getEmployeeId() {
        return employeeId;
    }

    public void setEmployeeId(Long employeeId) {
        this.employeeId = employeeId;
    }

    public String getEmployeePublicCode() {
        return employeePublicCode;
    }

    public void setEmployeePublicCode(String employeePublicCode) {
        this.employeePublicCode = employeePublicCode;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
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

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getCivilStatus() {
        return civilStatus;
    }

    public void setCivilStatus(String civilStatus) {
        this.civilStatus = civilStatus;
    }

    public String getHonorific() {
        return honorific;
    }

    public void setHonorific(String honorific) {
        this.honorific = honorific;
    }

    public String getBirthName() {
        return birthName;
    }

    public void setBirthName(String birthName) {
        this.birthName = birthName;
    }

    public String getCitizenship() {
        return citizenship;
    }

    public void setCitizenship(String citizenship) {
        this.citizenship = citizenship;
    }

    public String getNationalityDenonym() {
        return nationalityDenonym;
    }

    public void setNationalityDenonym(String nationalityDenonym) {
        this.nationalityDenonym = nationalityDenonym;
    }

    public String getReligion() {
        return religion;
    }

    public void setReligion(String religion) {
        this.religion = religion;
    }

    public String getDateOfBirth() {
        return dateOfBirth;
    }

    public void setDateOfBirth(String dateOfBirth) {
        this.dateOfBirth = dateOfBirth;
    }

    public String getEntryDate() {
        return entryDate;
    }

    public void setEntryDate(String entryDate) {
        this.entryDate = entryDate;
    }

    public String getExitDate() {
        return exitDate;
    }

    public void setExitDate(String exitDate) {
        this.exitDate = exitDate;
    }

    public String getDisabilityGrade() {
        return disabilityGrade;
    }

    public void setDisabilityGrade(String disabilityGrade) {
        this.disabilityGrade = disabilityGrade;
    }

    public String getDisabilityValidFrom() {
        return disabilityValidFrom;
    }

    public void setDisabilityValidFrom(String disabilityValidFrom) {
        this.disabilityValidFrom = disabilityValidFrom;
    }

    public String getDisabilityValidTo() {
        return disabilityValidTo;
    }

    public void setDisabilityValidTo(String disabilityValidTo) {
        this.disabilityValidTo = disabilityValidTo;
    }

    public List<Long> getSupervisorIds() {
        return supervisorIds;
    }

    public void setSupervisorIds(List<Long> supervisorIds) {
        this.supervisorIds = supervisorIds;
    }

    public List<Telephone> getTelephoneNumbers() {
        return telephoneNumbers;
    }

    public void setTelephoneNumbers(List<Telephone> telephoneNumbers) {
        this.telephoneNumbers = telephoneNumbers;
    }

    public List<Address> getAddresses() {
        return addresses;
    }

    public void setAddresses(List<Address> addresses) {
        this.addresses = addresses;
    }

    public List<EMail> getEmailAddresses() {
        return emailAddresses;
    }

    public void setEmailAddresses(List<EMail> emailAddresses) {
        this.emailAddresses = emailAddresses;
    }

    public String getSettlementArea() {
        return settlementArea;
    }

    public void setSettlementArea(String settlementArea) {
        this.settlementArea = settlementArea;
    }

    public OrganizationUnit getOrganizationUnit() {
        return organizationUnit;
    }

    public void setOrganizationUnit(OrganizationUnit organizationUnit) {
        this.organizationUnit = organizationUnit;
    }

    public List<String> getRoles() {
        return roles;
    }

    public void setRoles(List<String> roles) {
        this.roles = roles;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Employee employee = (Employee) o;
        return Objects.equals(status, employee.status) && Objects.equals(employeeId, employee.employeeId) && Objects.equals(employeePublicCode, employee.employeePublicCode) && Objects.equals(userName, employee.userName) && Objects.equals(title, employee.title) && Objects.equals(firstName, employee.firstName) && Objects.equals(lastName, employee.lastName) && Objects.equals(gender, employee.gender) && Objects.equals(civilStatus, employee.civilStatus) && Objects.equals(honorific, employee.honorific) && Objects.equals(birthName, employee.birthName) && Objects.equals(citizenship, employee.citizenship) && Objects.equals(nationalityDenonym, employee.nationalityDenonym) && Objects.equals(religion, employee.religion) && Objects.equals(dateOfBirth, employee.dateOfBirth) && Objects.equals(entryDate, employee.entryDate) && Objects.equals(exitDate, employee.exitDate) && Objects.equals(disabilityGrade, employee.disabilityGrade) && Objects.equals(disabilityValidFrom, employee.disabilityValidFrom) && Objects.equals(disabilityValidTo, employee.disabilityValidTo) && Objects.equals(supervisorIds, employee.supervisorIds) && Objects.equals(telephoneNumbers, employee.telephoneNumbers) && Objects.equals(addresses, employee.addresses) && Objects.equals(emailAddresses, employee.emailAddresses) && Objects.equals(settlementArea, employee.settlementArea) && Objects.equals(organizationUnit, employee.organizationUnit) && Objects.equals(roles, employee.roles);
    }

    @Override
    public int hashCode() {
        return Objects.hash(status, employeeId, employeePublicCode, userName, title, firstName, lastName, gender, civilStatus, honorific, birthName, citizenship, nationalityDenonym, religion, dateOfBirth, entryDate, exitDate, disabilityGrade, disabilityValidFrom, disabilityValidTo, supervisorIds, telephoneNumbers, addresses, emailAddresses, settlementArea, organizationUnit, roles);
    }

    @Override
    public String toString() {
        return "Employee{" +
                "status='" + status + '\'' +
                ", employeeId=" + employeeId +
                ", employeePublicCode='" + employeePublicCode + '\'' +
                ", userName='" + userName + '\'' +
                ", title='" + title + '\'' +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", gender='" + gender + '\'' +
                ", civilStatus='" + civilStatus + '\'' +
                ", honorific='" + honorific + '\'' +
                ", birthName='" + birthName + '\'' +
                ", citizenship='" + citizenship + '\'' +
                ", nationalityDenonym='" + nationalityDenonym + '\'' +
                ", religion='" + religion + '\'' +
                ", dateOfBirth='" + dateOfBirth + '\'' +
                ", entryDate='" + entryDate + '\'' +
                ", exitDate='" + exitDate + '\'' +
                ", disabilityGrade='" + disabilityGrade + '\'' +
                ", disabilityValidFrom='" + disabilityValidFrom + '\'' +
                ", disabilityValidTo='" + disabilityValidTo + '\'' +
                ", supervisorIds=" + supervisorIds +
                ", telephoneNumbers=" + telephoneNumbers +
                ", addresses=" + addresses +
                ", emailAddresses=" + emailAddresses +
                ", settlementArea='" + settlementArea + '\'' +
                ", organizationUnit=" + organizationUnit +
                ", roles=" + roles +
                '}';
    }
}