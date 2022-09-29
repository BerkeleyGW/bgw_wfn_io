
from collections import OrderedDict
from functools import cached_property
import re


re_kw = re.compile(r'([^ ]*?): \s*(.*)')


class Node(object):
    def __init__(self, parent=None):
        self.parent = parent
        self.children = []
        self.keywords = OrderedDict()
        self.raw_content = ''
        if self.parent is None:
            self.level = 0
        else:
            self.parent.type in ('root', 'group')
            self.parent.children.append(self)
            self.level = self.parent.level + 1

    @cached_property
    def type(self):
        if self.level==0:
            return 'root'
        if 'Group' in self.keywords.keys():
            return 'group'
        if 'Dataset' in self.keywords.keys():
            return 'dataset'
        raise Exception('Invalid node type: {}'.format(str(self)))

    def get_child(self, name):
        for child in self.children:
            if child.type == 'group':
                if child.keywords['Group'] == name:
                    return child
            if child.type == 'Dataset':
                if child.keywords['Dataset'] == name:
                    return child
        return None

    def get_parent_with_level(self, level):
        '''Return parent node of level `level`'''
        if level > self.level:
            raise ValueError('Requested level larger than node level.')
        node = self
        while node.level > level:
            node = node.parent
        return node

    def get_full_path(self):
        path = []
        node = self
        while node.level > 0:
            if node.type=='dataset':
                s = node.keywords['Dataset']
            else:
                s = node.keywords['Group']
            path.insert(0, s)
            node = node.parent
        return path

    def __repr__(self):
        s = '{}<node level={} len(children)={} len(keywords)={}/>'.format(
                '  '*self.level, self.level, len(self.children), len(self.keywords))
        return s

    def __str__(self):
        if len(self.children):
            s = '{}<node level={} len(children)={} len(keywords)={}>\n'.format(
                    '  '*self.level, self.level, len(self.children), len(self.keywords))
            for child in self.children:
                s += str(child)
            s += '{}</node>\n'.format('  '*self.level)
        else:
            s = repr(self) + '\n'
        return s

    def __iter__(self):
        yield self
        for child in self.children:
            yield from child

    @cached_property
    def name(self):
        if self.type == 'root':
            return ''
        elif self.type == 'group':
            return self.keywords['Group']
        elif self.type == 'dataset':
            return self.keywords['Dataset']

    @cached_property
    def dtype(self):
        return self.keywords['Type']

    @cached_property
    def rank(self):
        return int(self.keywords['Rank'])

    @cached_property
    def dims(self):
        return [self.keywords[f'Dims({i+1})'] for i in range(self.rank)]


def parse_h5_spec(inp_fname):
    # Parse spec file
    root = Node()
    last_group = root
    cur_group = None
    last_kw = None
    with open(inp_fname, 'r') as f:
        for line_orig in f.readlines():
            line_orig = line_orig.rstrip()
            line = line_orig.strip('\t')
            if len(line)==0:
                # Got an empty line: create new group
                if cur_group is not None:
                    last_group = cur_group
                cur_group = None
                last_kw = None
                continue
            if line[0]=='#':
                # Ignore comments
                continue

            level = line_orig.count('\t') + 1
            if cur_group is None:
                # Create a new group. First, find the parent.
                parent = last_group.get_parent_with_level(level-1)
                cur_group = Node(parent)

            cur_group.raw_content += line
            match = re_kw.match(line)
            if match is None:
                # We are continuing the definition of a keyword.
                assert last_kw is not None
                cur_group.keywords[last_kw] += " " + line
            else:
                # New keyword
                last_kw = match.group(1)
                text = match.group(2)
                cur_group.keywords[last_kw] = text

    return root
