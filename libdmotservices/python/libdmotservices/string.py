from typing import Optional

def get_resource_base_name(loc: Optional[str]) -> Optional[str]:
    """Return the rightmost component of a resource path.

    Supports both POSIX ``/`` and Windows ``\\`` separators. ``None`` or empty
    inputs result in ``None`` so callers can safely propagate optional values.

    Example:
        >>> get_resource_base_name("assets/images/logo.png")
        'logo.png'
    """
    if not loc:
        return None
    split_char = "/"
    if "\\" in loc:
        split_char = "\\"
    return loc.rsplit(split_char, 1)[-1]