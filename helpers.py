import re
import sys

def parse_input(line: str):

    """Parse input line into command, street name, and vertices"""
    pattern1 = r'\S+\s+"[^"]+"\s\([^)]+\)\s+\([^)]+\)(?:\s+\([^)]+\))*'
    pattern2 = r'^\S*'
    if not re.search(pattern1, line) and not re.search(pattern2, line):
        print('Error: You did not obey the requested format', file=sys.stderr)
        return None, None, None
    inputs = re.split("\s", line)
    if len(inputs) < 4 and inputs[0] != "gg" and inputs[0] != "rm":
        print('Error: You did not obey the requested format', file=sys.stderr)
        return None, None, None
    cmd = inputs[0]
    if cmd in ["add", "mod", "rm", "gg"]:
        if cmd == "gg" and len(inputs) > 2:
            print('Error: You did not obey the requested format', file=sys.stderr)
            return None, None, None
        if cmd != "gg":
            inputs = re.split('"', line)
            
            try:
                street_name = inputs[1]
            except:
                print('Error: You did not obey the requested format', file=sys.stderr)
                return None, None, None
            if cmd != "rm":
                pattern = r'\([^)]+\)[^\(\)\s]+\([^)]+\)'
                if re.search(pattern, inputs[-1].strip()):
                    print('Error: You did not obey the requested format', file=sys.stderr)
                vertices = re.findall(r'\([-+]?\s*\d+\s*,\s*[-+]?\s*\d+\s*\)', inputs[-1].strip()) 
                return cmd, street_name, vertices
            return cmd, street_name, None
        else:
            return cmd, None, None
    elif cmd not in ["add", "mod", "rm", "gg"]:
        print('Error: Invalid command', file=sys.stderr)
        return None, None, None

def create_line(point1, point2):
    """Create a line from two points, considering the line equation Ax + By + C = 0"""
    A = (point1[1] - point2[1])
    B = (point2[0] - point1[0])
    C = (point1[0]*point2[1] - point2[0]*point1[1])
    return A, B, -C

def intersection(line1, line2):
    """Find the intersection point of two lines, considering the line equation Ax + By + C = 0
    and using Cramer's rule"""
    D = line1[0] * line2[1] - line1[1] * line2[0]
    Dx = line1[2] * line2[1] - line1[1] * line2[2]
    Dy = line1[0] * line2[2] - line1[2] * line2[0]
    if D != 0:
        x = Dx / D
        y = Dy / D
        return round(x, 2), round(y, 2)
    else:
        return False
    
def check_intersection(intersection_point, end1_line1, end2_line1, end1_line2,
                        end2_line2):
    """Check if the intersection point is on the line segment"""
    if intersection_point[0] >= min(end1_line1[0], end2_line1[0]) and \
        intersection_point[0] <= max(end1_line1[0], end2_line1[0]) and \
        intersection_point[0] >= min(end1_line2[0], end2_line2[0]) and \
        intersection_point[0] <= max(end1_line2[0], end2_line2[0]) and \
        intersection_point[1] >= min(end1_line1[1], end2_line1[1]) and \
        intersection_point[1] <= max(end1_line1[1], end2_line1[1]) and \
        intersection_point[1] >= min(end1_line2[1], end2_line2[1]) and \
        intersection_point[1] <= max(end1_line2[1], end2_line2[1]):
        return True
    else:
        return False

    