#ifndef WORD_H
#define WORD_H


class Word
{
    public:
        unsigned long long key;
        unsigned int pos;
        void set_key (unsigned long long);
        void set_pos (unsigned int);
        bool operator< (const Word& w) const { return key < w.key; }
    protected:

    private:
};

#endif // WORD_H
