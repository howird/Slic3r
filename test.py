import build.pyslic3r as sl

def cfgs():
    cd = sl.ConfigDef()
    dc = sl.DynamicConfig(cd)

    dc.load('test/inputs/test_gcodewriter/config_lift_unlift.ini')
    # dc.save('dc')

def print_cfgs():
    dc = sl.DynamicPrintConfig()
    pc = sl.FullPrintConfig()

    dc.load('test/inputs/test_gcodewriter/config_lift_unlift.ini')
    pc.apply(dc)

    # dc.save('dc')
    # pc.save('pc')

if __name__ == '__main__':
    cfgs()
    print_cfgs()