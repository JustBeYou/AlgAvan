from numpy import log2, ceil
from numpy.random import uniform
from random import choice, randrange
from operator import itemgetter
from matplotlib import pyplot as plt


def main():
    # Datele de intrare se pot stabili in parametrii obiectului Environment
    env = Environment(
            fitness='x**3+3 * x**2 - 4*x + 7',
            pop_size=10, domain=(-4, 3), iterations=5000, precision=10)
    env.train()


class Environment:
    def __init__(
            self,
            pop_size=20, # dimensiunea populatiei
            domain=(-1, 2), # domeniul
            fitness='-x**2 + x + 2', # functia de maximizat
            precision=6, # precizia p
            crossover_prob=0.25, # rata de imperechere
            mutation_prob=0.05, # rata de mutatie per gena
            iterations=50): # iteratii
        self.pop_size = pop_size
        self.domain = domain
        self.fitness = compile(fitness, __name__, 'eval')
        self.precision = precision
        self.crossover_prob = crossover_prob
        self.mutation_prob = mutation_prob
        self.iterations = iterations
        # calculam lungimea reprezentarii
        self.length = int(ceil(log2((domain[1] - domain[0]) * 10**precision)))

        # istoric evolutie pentru afisare
        self.first = True
        self.points = []
        self.mean_points = []

    def train(self):
        # generatia initiala
        self.genesis()
        for iteration in range(self.iterations):
            # calculam probabilitatile, supravietuitorii si imperecherea
            probs = self.probabilities()
            intervals = partial_sums([0] + [v for k, v in probs.items()])
            survivors = self.select(intervals)
            mates = self.select_mates(survivors)

            if self.first:
                print("Probabilitati supravietuire")
                for i, (k, v) in enumerate(probs.items()):
                    print(f"cromozom {i} p= {v:.5f}")
                print()

                print("Intervale probabilitati supravietuire")
                print(intervals)
                print()

            survivors = [list(probs.keys())[i] for i in survivors]

            if self.first:
                print("Supravietuitori")
                for i, v in enumerate(survivors):
                    print(f"{i}: {v}")
                print()

            offspring = self.mate(mates, survivors)

            if self.first:
                print("Imperechere")
                j = 0
                for i in range(0, len(mates), 2):
                    if i + 1 >= len(mates):
                        break
                    print(f"Imperechere intre {mates[i]} si {mates[i+1]}")
                    print(f"Copii: {offspring[j][0]} si {offspring[j][1]}")
                    print()
                    j += 1

            # Copiii pot suferi mutatii
            for X, Y in offspring:
                X.mutate(self.mutation_prob)
                X.mutate_by_inversion(self.mutation_prob)
                Y.mutate(self.mutation_prob)
                Y.mutate_by_inversion(self.mutation_prob)

            j = 0
            for i in range(0, len(mates), 2):
                if i + 1 >= len(mates):
                    break
                survivors[mates[i]] = offspring[j][0]
                survivors[mates[i+1]] = offspring[j][1]
                j += 1

            # Adaugam cel mai bun exemplar in noua generatie
            # (selectie elitista)
            survivors = [
                    list(self.population.keys())[self.fittest()]
                    ] + survivors

            self.population = {X: self.evaluate(X) for X in survivors}

            if self.first:
                print("Generatia urmatoare dupa mutatie")
                for i, (k, v) in enumerate(self.population.items()):
                    print(f"{i}: {k} f= {v:.5f}")
                print()

            if self.first:
                print("Performanta maxima si medie")

            best = list(self.population.items())[self.fittest()]
            #self.points.append(self.evaluate(best[0]))

            mean = sum([v for k, v in self.population.items()]) / len(self.population.items())
            self.mean_points.append(mean)

            print(f"Maxima {best} Medie {mean}")

            self.first = False if self.first else self.first

        # Grafice evolutie
        plt.title('Performanta maxima')
        plt.plot(list(range(len(self.points))), self.points)
        plt.xlabel('Generatii')
        plt.ylabel('Performanta')
        plt.show()

        plt.title('Performanta medie')
        plt.plot(list(range(len(self.mean_points))), self.mean_points)
        plt.xlabel('Generatii')
        plt.ylabel('Performanta')
        plt.show()

    def genesis(self):
        # Creem cromozomi cu configuratii aleatoare
        self.population = [
                Chromosome(
                    random_string('01', self.length),
                    self.domain) for _ in range(self.pop_size)
            ]
        self.population = {X: self.evaluate(X) for X in self.population}

        if self.first:
            print("Generatia initiala")
            for i, (k, v) in enumerate(self.population.items()):
                print(f"{i}: {k} f= {v:.5f}")
            print()

    def evaluate(self, X):
        return eval(self.fitness, {'x': X.value()})

    def fittest(self):
        # Cautam cel mai bun exemplar
        return list(self.population.keys()).index(max(
                self.population.items(),
                key=itemgetter(1)
            )[0])

    def probabilities(self):
        # Probabilitaile de supravietuire
        total = sum([v for k, v in self.population.items()])
        return {
                X: (value / total) for X, value in self.population.items()
            }

    def select(self, intervals):
        # Selectia aleatoare
        return [
            interval_search(
                uniform(),
                intervals
            ) for _ in range(self.pop_size)
        ]

    def select_mates(self, survivors):
        # Generarea perechilor
        return [
            i for i in range(len(survivors)) if uniform() < self.crossover_prob
        ]

    def mate(self, mates, pop):
        # Generarea de noi copii
        offspring = []
        for i in range(0, len(mates), 2):
            if i + 1 >= len(mates):
                break
            offspring.append(pop[mates[i]] + pop[mates[i+1]])
        return offspring


def partial_sums(lst):
    sums = lst.copy()
    for i in range(1, len(lst)):
        sums[i] += sums[i-1]
    return sums


def random_string(chars, length):
    return ''.join(choice(chars) for _ in range(length))

# Cautare binara in intervale
def interval_search(u, intervals):
    left, right = 0, len(intervals)

    while left <= right:
        mid = (left + right) // 2
        if (mid + 1 < len(intervals) and
           intervals[mid] <= u and u < intervals[mid + 1]):
            return mid
        elif u < intervals[mid]:
            right = mid - 1
        elif mid + 1 < len(intervals) and u >= intervals[mid + 1]:
            left = mid + 1

    return None


class Chromosome:
    def __init__(self, bits, domain):
        self.bits = bits
        self.x_10 = int(bits, 2)
        self.domain = domain
        self.length = len(bits)

    def value(self):
        # Transformare reprezentare binara -> numar
        return ((self.domain[1] - self.domain[0]) /
                (2**self.length - 1) * self.x_10 +
                self.domain[0])

    def mutate(self, prob):
        # Mutare aleatoare gene
        new_bits = [bit for bit in self.bits]
        for i in range(len(new_bits)):
            if uniform() < prob:
                if new_bits[i] == '1':
                    new_bits[i] = '0'
                else:
                    new_bits[i] = '1'
        self.bits = ''.join(new_bits)

    def mutate_by_inversion(self, prob):
        if uniform() < prob: return

        points = [randrange(0, self.length) for _ in range(2)]
        points = sorted(points)

        if points[0] == points[1]: return

        left = self.bits[:points[0]]
        middle = self.bits[points[0]:points[1]:-1]
        right = self.bits[points[1]:]

        self.bits = left + middle + right

    def __add__(self, rhs):
        # Imperechere cu o singura taietura
        cut = randrange(0, self.length)
        return (
            Chromosome(self.bits[:cut] + rhs.bits[cut:], self.domain),
            Chromosome(self.bits[cut:] + rhs.bits[:cut], self.domain)
        )

    def __repr__(self):
        return f"{self.bits} x= {self.value():.5f}"


if __name__ == "__main__":
    # Teste
    assert Environment().length == 22
    assert (Chromosome("0000011101001001110001", (-1, 2)).value() ==
            -0.9145920073013323)
    assert interval_search(0.3, [
        0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7
    ]) == 3
    assert partial_sums([1, 2, 3, 4]) == [1, 3, 6, 10]

    main()
