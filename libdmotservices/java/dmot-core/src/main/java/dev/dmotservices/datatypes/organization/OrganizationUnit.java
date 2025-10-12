package dev.dmotservices.datatypes.organization;

import java.io.Serializable;
import java.util.Objects;

public class OrganizationUnit implements Serializable {

    protected String oid;
    protected String parentId;
    protected DepartmentLevel departmentLevel;
    protected String name;

    public OrganizationUnit() {

    }

    /*
     * generated
     */

    public String getOid() {
        return oid;
    }

    public void setOid(String oid) {
        this.oid = oid;
    }

    public String getParentId() {
        return parentId;
    }

    public void setParentId(String parentId) {
        this.parentId = parentId;
    }

    public DepartmentLevel getDepartmentLevel() {
        return departmentLevel;
    }

    public void setDepartmentLevel(DepartmentLevel departmentLevel) {
        this.departmentLevel = departmentLevel;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        OrganizationUnit that = (OrganizationUnit) o;
        return Objects.equals(oid, that.oid) && Objects.equals(parentId, that.parentId) && departmentLevel == that.departmentLevel && Objects.equals(name, that.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(oid, parentId, departmentLevel, name);
    }

    @Override
    public String toString() {
        return "OrganizationUnit{" +
                "oid='" + oid + '\'' +
                ", parentId='" + parentId + '\'' +
                ", departmentLevel=" + departmentLevel +
                ", name='" + name + '\'' +
                '}';
    }
}
